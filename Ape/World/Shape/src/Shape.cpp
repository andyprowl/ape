#include <Ape/World/Shape/Shape.hpp>

#include <Glow/BufferObject/VertexArrayObject.hpp>
#include <Glow/BufferObject/VertexLayout.hpp>

#include <glad/glad.h>
#include <glm/geometric.hpp>

namespace ape
{

namespace
{

auto const minFloat = std::numeric_limits<float>::min();

auto const maxFloat = std::numeric_limits<float>::max();

auto makeVertexBufferObject(std::vector<ShapeVertex> const & vertices)
    -> glow::VertexBufferObject
{
    auto vbo = glow::VertexBufferObject{};

    vbo.bind();

    auto const vertexBufferSize = vertices.size() * sizeof(ShapeVertex);

    glBufferData(GL_ARRAY_BUFFER, vertexBufferSize, vertices.data(), GL_STATIC_DRAW);

    glow::sendVertexLayoutToGpu<ShapeVertex>();

    return vbo;
}

auto makeVertexIndexBufferObject(std::vector<unsigned int> const & indices)
    -> glow::ElementBufferObject
{
    auto ebo = glow::ElementBufferObject{};

    ebo.bind();

    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        indices.size() * sizeof(unsigned int),
        indices.data(),
        GL_STATIC_DRAW);

    return ebo;
}

auto makeAxisAlignedBoundingBox(std::vector<ShapeVertex> const & vertices)
    -> Box
{
    auto min = glm::vec3{maxFloat, maxFloat, maxFloat};
    
    auto max = glm::vec3{minFloat, minFloat, minFloat};

    for (auto const & vertex : vertices)
    {
        auto const & pos = vertex.position;

        min = {std::min(min.x, pos.x), std::min(min.y, pos.y), std::min(min.z, pos.z)};

        max = {std::max(max.x, pos.x), std::max(max.y, pos.y), std::max(max.z, pos.z)};
    }

    return {(min + max) / 2.0f, {max - min}};
}

auto makeBoundingSphere(Box const & box)
    -> Sphere
{
    auto const min = box.getCorner(Box::Corner::leftBottomBack);

    auto const max = box.getCorner(Box::Corner::rightTopFront);

    auto const center = (max + min) / 2.0f;

    auto const radius = glm::length(max - center);

    return {center, radius};
}

auto makeBoundingVolumes(std::vector<ShapeVertex> const & vertices)
    -> ShapeBounds
{
    auto const box = makeAxisAlignedBoundingBox(vertices);

    auto const sphere = makeBoundingSphere(box);

    return {box, sphere};
}

} // unnamed namespace

Shape::Shape(std::vector<ShapeVertex> const & vertices, std::vector<unsigned int> const & indices)
    : bufferObjects{makeVertices(vertices, indices)}
    , numOfVertices{static_cast<int>(indices.size())}
    , boundingVolumes{makeBoundingVolumes(vertices)}
{
}

auto Shape::getVertexBufferObject() const 
    -> const glow::VertexBufferObject &
{
    return bufferObjects.vertex;
}

auto Shape::getElementBufferObject() const 
    -> const glow::ElementBufferObject &
{
    return bufferObjects.element;
}

auto Shape::getNumOfVertices() const
    -> int
{
    return numOfVertices;
}

auto Shape::getBoundingVolumes() const
    -> ShapeBounds const &
{
    return boundingVolumes;
}

auto Shape::makeVertices(
    std::vector<ShapeVertex> const & vertices,
    std::vector<unsigned int> const & indices) const
    -> BufferObjectSet
{
    auto vao = glow::VertexArrayObject{};

    vao.bind();

    auto vbo = makeVertexBufferObject(vertices);

    auto ebo = makeVertexIndexBufferObject(indices);

    vao.unbind();

    return {std::move(vbo), std::move(ebo)};
}

} // namespace ape
