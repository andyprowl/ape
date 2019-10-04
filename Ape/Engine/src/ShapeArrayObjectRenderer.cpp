#include <Engine/ShapeArrayObjectRenderer.hpp>

#include <Asset/AssetRepository.hpp>

#include <Range/Insertion.hpp>

#include <glad/glad.h>

#include <algorithm>

namespace ape
{

namespace
{

auto setupRenderingState(Shape const & shape)
    -> void
{
    shape.getVertexBufferObject().bind();

    sendVertexLayoutToGpu();

    shape.getElementBufferObject().bind();
}

} // unnamed namespace

ShapeArrayObjectRenderer::ShapeArrayObjectRenderer(std::vector<AssetRepository const *> assets)
    : assets{std::move(assets)}
    , shapeArrayObjects{makeShapeArrayObjects()}
{
}

// virtual (from ShapeRenderer)
auto ShapeArrayObjectRenderer::render(Shape const & shape) const
    -> void
{
    auto & arrayObject = getArrayObjectForShape(shape);

    arrayObject.bind();

    auto const numOfVertices = shape.getNumOfVertices();

    glDrawElements(GL_TRIANGLES, numOfVertices, GL_UNSIGNED_INT, 0);

    arrayObject.unbind();
}

auto ShapeArrayObjectRenderer::makeShapeArrayObjects() const
    -> std::vector<VertexArrayObject>
{
    auto arrayObjects = std::vector<VertexArrayObject>{};

    arrayObjects.resize(Counted<Shape>::getNumOfInstances());

    for (auto const repository : assets)
    {
        setupArrayObjectsForShapes(*repository, arrayObjects);
    }

    return arrayObjects;
}

auto ShapeArrayObjectRenderer::setupArrayObjectsForShapes(
    AssetRepository const & repository,
    std::vector<VertexArrayObject> & destination) const
    -> void
{
    for (auto const & shape : repository.shapes)
    {
        auto const id = shape.getInstanceIndex();

        auto & shapeArrayObject = destination[id];

        shapeArrayObject.bind();

        setupRenderingState(shape);

        shapeArrayObject.unbind();
    }
}

auto ShapeArrayObjectRenderer::getArrayObjectForShape(Shape const & shape) const
    -> const VertexArrayObject &
{
    auto const shapeIndex = shape.getInstanceIndex();

    return shapeArrayObjects[shapeIndex];
}
} // namespace ape
