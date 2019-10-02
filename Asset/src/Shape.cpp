#include <Asset/Shape.hpp>

#include <Asset/RenderingContext.hpp>

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

#include <algorithm>
#include <array>
#include <cassert>
#include <numeric>
#include <tuple>

namespace ape
{

namespace
{

#define encodeComponentOffset(component) (void*)(offsetof(Vertex, component))

auto setVertexAttribute(int const position, int const numOfFloats, void * componentOffset)
    -> void
{
    auto const stride = sizeof(Vertex);

    glVertexAttribPointer(position, numOfFloats, GL_FLOAT, GL_FALSE, stride, componentOffset);

    glEnableVertexAttribArray(position);
}

auto makeVertexBufferObject(std::vector<Vertex> const & vertices)
    -> VertexBufferObject
{
    auto vbo = VertexBufferObject{};

    vbo.bind();

    auto const vertexBufferSize = vertices.size() * sizeof(Vertex);

    glBufferData(GL_ARRAY_BUFFER, vertexBufferSize, vertices.data(), GL_STATIC_DRAW);

    setVertexAttribute(0, 3, encodeComponentOffset(position));

    setVertexAttribute(1, 3, encodeComponentOffset(normal));

    setVertexAttribute(2, 2, encodeComponentOffset(textureCoordinates));

    return vbo;
}

auto makeVertexIndexBufferObject(std::vector<unsigned int> const & indices)
    -> ElementBufferObject
{
    auto ebo = ElementBufferObject{};

    ebo.bind();

    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        indices.size() * sizeof(unsigned int),
        indices.data(),
        GL_STATIC_DRAW);

    return ebo;
}

auto makeVertices(
    std::vector<Vertex> const & vertices,
    std::vector<unsigned int> const & indices)
    -> Shape::BufferSet
{
    auto vao = VertexArrayObject{};

    vao.bind();

    auto vbo = makeVertexBufferObject(vertices);

    auto ebo = makeVertexIndexBufferObject(indices);

    vao.unbind();

    return {std::move(vbo), std::move(ebo)};
}

auto computeCenter(std::vector<Vertex> const & vertices)
    -> glm::vec3
{
    auto sum = glm::vec3{0.0f, 0.0f, 0.0f};

    for (auto const & vertex : vertices)
    {
        sum += vertex.position;
    }

    return sum * (1.0f / vertices.size());
}

} // unnamed namespace

Shape::Shape(std::vector<Vertex> const & vertices, std::vector<unsigned int> const & indices)
    : buffers{makeVertices(vertices, indices)}
    , numOfVertices{static_cast<int>(indices.size())}
    , center{computeCenter(vertices)}
{
}

auto Shape::getCenter() const
    -> glm::vec3
{
    return center;
}

auto Shape::draw(RenderingContext const & context) const
    -> void
{
    if (context.getPolicy() == RenderingPolicy::doNotUseArrayObjects)
    {
        drawWithoutArrayVertexObject();
    }
    else
    {
        drawWithArrayVertexObject(context);
    }
}

auto Shape::drawWithoutArrayVertexObject() const
    -> void
{
    setupRenderingState();

    glDrawElements(GL_TRIANGLES, numOfVertices, GL_UNSIGNED_INT, 0);
}

auto Shape::drawWithArrayVertexObject(RenderingContext const & context) const
    -> void
{
    auto & vertexArrayObject = retrieveBoundVertexArrayObject(context);
    
    glDrawElements(GL_TRIANGLES, numOfVertices, GL_UNSIGNED_INT, 0);

    vertexArrayObject.unbind();
}

auto Shape::retrieveBoundVertexArrayObject(RenderingContext const & context) const
    -> VertexArrayObject &
{
    if (hasVertexArrayObject(context))
    {
        return bindExistingVertexArrayObject(context);
    }
    else
    {
        return createNewVertexArrayObject(context);
    }
}

auto Shape::hasVertexArrayObject(RenderingContext const & context) const
    -> bool
{
    return (context.getId() < static_cast<int>(contextArrayObjectMap.size()));
}

auto Shape::bindExistingVertexArrayObject(RenderingContext const & context) const
    -> VertexArrayObject &
{
    auto & arrayObject = contextArrayObjectMap[context.getId()];

    arrayObject.bind();

    return arrayObject;
}

auto Shape::createNewVertexArrayObject(RenderingContext const & context) const
    -> VertexArrayObject &
{
    contextArrayObjectMap.resize(static_cast<std::size_t>(context.getId()) + 1u);

    auto & newArrayObject = contextArrayObjectMap.back();

    newArrayObject.bind();

    setupRenderingState();

    return newArrayObject;
}

auto Shape::setupRenderingState() const
    -> void
{
    buffers.vertexBufferObject.bind();

    setVertexAttribute(0, 3, encodeComponentOffset(position));

    setVertexAttribute(1, 3, encodeComponentOffset(normal));

    setVertexAttribute(2, 2, encodeComponentOffset(textureCoordinates));

    buffers.elementBufferObject.bind();
}

} // namespace ape
