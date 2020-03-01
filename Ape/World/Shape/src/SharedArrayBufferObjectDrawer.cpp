#include <Ape/World/Shape/SharedArrayBufferObjectDrawer.hpp>

#include <Ape/World/Shape/Shape.hpp>
#include <Ape/World/Shape/ShapeBufferObjectCreator.hpp>

#include <Glow/BufferObject/VertexLayout.hpp>
#include <Glow/GpuResource/ScopedBinder.hpp>

#include <Basix/Meta/Tag.hpp>
#include <Basix/Range/Insertion.hpp>

#include <glad/glad.h>

#include <algorithm>
#include <numeric>

namespace ape
{

namespace
{

auto asVoidPointer(int const value)
    -> void *
{
    return reinterpret_cast<void *>(static_cast<std::uintptr_t>(value));
}

auto appendVertexBytes(std::vector<std::byte> & buffer, std::vector<ShapeVertex> const & vertices)
    -> void
{
    auto const begin = reinterpret_cast<std::byte const *>(vertices.data());

    auto const end = std::next(begin, vertices.size() * sizeof(ShapeVertex));

    buffer.insert(std::end(buffer), begin, end);
}

template<typename T>
auto appendIndexBytes(std::vector<std::byte> & buffer, std::vector<unsigned int> const & indices)
    -> void
{
    buffer.reserve(buffer.size() + indices.size() * sizeof(T));

    for (auto const index : indices)
    {
        auto const castIndex = static_cast<T>(index);

        std::copy(
            reinterpret_cast<std::byte const *>(&castIndex),
            reinterpret_cast<std::byte const *>(&castIndex) + sizeof(T),
            std::back_inserter(buffer));
    }
}

auto appendIndexBytes(
    std::vector<std::byte> & buffer,
    std::vector<ShapeVertex> const & vertices,
    std::vector<unsigned int> const & indices)
    -> void
{
//   appendIndexBytes<std::uint16_t>(buffer, indices);

    if (vertices.size() > std::numeric_limits<std::uint16_t>::max())
    {
        appendIndexBytes<std::uint32_t>(buffer, indices);
    }
    else if (vertices.size() > std::numeric_limits<std::uint8_t>::max())
    {
        appendIndexBytes<std::uint16_t>(buffer, indices);
    }
    else
    {
        appendIndexBytes<std::uint8_t>(buffer, indices);
    }
}

auto getIndexElementType(std::vector<ShapeVertex> const & vertices)
    -> int
{
//    return GL_UNSIGNED_SHORT;

    if (vertices.size() > std::numeric_limits<std::uint16_t>::max())
    {
        return GL_UNSIGNED_INT;
    }
    else if (vertices.size() > std::numeric_limits<std::uint8_t>::max())
    {
        return GL_UNSIGNED_SHORT;
    }
    else
    {
        return GL_UNSIGNED_BYTE;
    }
}

auto makeVertexBufferObject(std::vector<std::byte> const & data)
    -> glow::VertexBufferObject
{
    auto vbo = glow::VertexBufferObject{};

    auto const vertexBufferData = static_cast<void const *>(data.data());

    auto const vertexBufferSize = data.size();

    vbo.createStorage(vertexBufferData, vertexBufferSize);

    return vbo;
}

auto makeIndexBufferObject(std::vector<std::byte> const & data)
    -> glow::ElementBufferObject
{
    auto ebo = glow::ElementBufferObject{};

    auto const indexBufferData = static_cast<void const *>(data.data());

    auto const indexBufferSize = data.size();

    ebo.createStorage(indexBufferData, indexBufferSize);

    return ebo;
}

} // unnamed namespace

SharedArrayBufferObjectDrawer::SharedArrayBufferObjectDrawer(std::vector<Shape *> const & shapes)
    : SharedArrayBufferObjectDrawer{mergeShapeGeometry(shapes)}
{
}

// virtual (from ShapeDrawer)
auto SharedArrayBufferObjectDrawer::beginDrawBatch()
    -> void
{
    bufferObjects.array.bind();
}

// virtual (from ShapeDrawer)
auto SharedArrayBufferObjectDrawer::draw(Shape const & shape)
    -> void
{
    auto const & bufferPortion = getBufferPortionForShape(shape);
    
    auto const numOfIndices = getNumOfIndices(shape);

    glDrawElementsBaseVertex(
        GL_TRIANGLES,
        numOfIndices,
        bufferPortion.indexBufferElementType,
        asVoidPointer(bufferPortion.indexBufferByteOffset),
        bufferPortion.vertexBufferOffset);
}

// virtual (from ShapeDrawer)
auto SharedArrayBufferObjectDrawer::endDrawBatch()
    -> void
{
    bufferObjects.array.unbind();
}

SharedArrayBufferObjectDrawer::SharedArrayBufferObjectDrawer(ShapeGeometry geometry)
    : bufferObjects{
        glow::VertexArrayObject{},
        makeVertexBufferObject(geometry.vertexBufferData),
        makeIndexBufferObject(geometry.indexBufferData)}
    , bufferPortions{std::move(geometry.bufferPortions)}
{
    setupVertexFormat();
}

auto SharedArrayBufferObjectDrawer::getBufferPortionForShape(Shape const & shape) const
    -> const ShapeBufferPortion &
{
    auto const shapeIndex = shape.getInstanceIndex();

    return bufferPortions[shapeIndex];
}

auto SharedArrayBufferObjectDrawer::mergeShapeGeometry(std::vector<Shape *> const & shapes) const
    -> ShapeGeometry
{
    auto geometry = ShapeGeometry{};

    auto bufferPortion = ShapeBufferPortion{0, 0, 0};

    for (auto const shape : shapes)
    {
        bufferPortion.indexBufferElementType = getIndexElementType(shape->getVertices());

        addPaddingToAlignNextIndexBufferPortion(geometry, bufferPortion);

        appendVertexBytes(geometry.vertexBufferData, shape->getVertices());

        appendIndexBytes(geometry.indexBufferData, shape->getVertices(), shape->getIndices());

        geometry.bufferPortions.push_back(bufferPortion);

        bufferPortion.vertexBufferOffset += static_cast<int>(shape->getVertices().size());

        bufferPortion.indexBufferByteOffset = static_cast<int>(geometry.indexBufferData.size());
    }

    return geometry;
}

auto SharedArrayBufferObjectDrawer::addPaddingToAlignNextIndexBufferPortion(
    ShapeGeometry & geometry,
    ShapeBufferPortion & bufferPortion) const
    -> void
{
    if (bufferPortion.indexBufferElementType == GL_UNSIGNED_INT)
    {
        addPaddingToAlignNextIndexBufferPortion<4>(geometry, bufferPortion);
    }
    else if (bufferPortion.indexBufferElementType == GL_UNSIGNED_SHORT)
    {
        addPaddingToAlignNextIndexBufferPortion<2>(geometry, bufferPortion);
    }
}

template<int IndexElementSize>
auto SharedArrayBufferObjectDrawer::addPaddingToAlignNextIndexBufferPortion(
    ShapeGeometry & geometry,
    ShapeBufferPortion & bufferPortion) const
    -> void
{
    auto const misalignment = geometry.indexBufferData.size() % IndexElementSize;

    if (misalignment == 0)
    {
        return;
    }

    auto const padding = IndexElementSize - misalignment;

    std::fill_n(std::back_inserter(geometry.indexBufferData), padding, std::byte{0});

    bufferPortion.indexBufferByteOffset += static_cast<int>(padding);
}

auto SharedArrayBufferObjectDrawer::setupVertexFormat()
    -> void
{
    auto const layout = glow::getVertexLayout<ShapeVertex>();

    auto const & attributes = layout.getAttributes();

    for (auto i = 0; i < static_cast<int>(attributes.size()); ++i)
    {
        auto binding = bufferObjects.array.getBinding(i);

        binding.setAttributeIndex(i);

        binding.setFormat(attributes[i]);
        
        binding.setVertexSource(bufferObjects.vertex, 0, sizeof(ShapeVertex));

        bufferObjects.array.enableAttribute(i);
    }

    bufferObjects.array.setIndexSource(bufferObjects.element);
}

} // namespace ape
