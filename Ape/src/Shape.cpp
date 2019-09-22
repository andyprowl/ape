#include <Ape/Shape.hpp>

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

#include <algorithm>
#include <array>
#include <numeric>
#include <tuple>

namespace ape
{

namespace
{

#define encodeComponentOffset(component) (void*)(offsetof(Vertex, component))

auto setAttribute(int const position, int const numOfFloats, void * componentOffset)
    -> void
{
    auto const stride = sizeof(Vertex);

    glVertexAttribPointer(position, numOfFloats, GL_FLOAT, GL_FALSE, stride, componentOffset);

    glEnableVertexAttribArray(position);
}

auto makeVertexBufferObject(std::vector<Vertex> const & vertices)
    -> unsigned int
{
    auto vboId = static_cast<unsigned int>(0);

    glGenBuffers(1, &vboId);

    glBindBuffer(GL_ARRAY_BUFFER, vboId);

    auto const vertexBufferSize = vertices.size() * sizeof(Vertex);

    glBufferData(GL_ARRAY_BUFFER, vertexBufferSize, vertices.data(), GL_STATIC_DRAW);

    setAttribute(0, 3, encodeComponentOffset(position));

    setAttribute(1, 3, encodeComponentOffset(normal));

    setAttribute(2, 2, encodeComponentOffset(textureCoordinates));

    return vboId;
}

auto makeVertexIndexBufferObject(std::vector<unsigned int> const & indices)
    -> unsigned int
{
    auto eboId = static_cast<unsigned int>(0);

    glGenBuffers(1, &eboId);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboId);

    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        indices.size() * sizeof(unsigned int),
        indices.data(),
        GL_STATIC_DRAW);

    return eboId;
}

auto makeVertices(
    std::vector<Vertex> const & vertices,
    std::vector<unsigned int> const & indices)
    -> Shape::ObjectIdSet
{
    auto vaoId = static_cast<unsigned int>(0);

    glGenVertexArrays(1, &vaoId);

    glBindVertexArray(vaoId);

    auto const vboId = makeVertexBufferObject(vertices);

    auto const eboId = makeVertexIndexBufferObject(indices);

    glBindVertexArray(0);

    return {vboId, eboId, vaoId};
}

} // unnamed namespace

Shape::Shape(std::vector<Vertex> const & vertices, std::vector<unsigned int> const & indices)
    : objectIds{makeVertices(vertices, indices)}
    , numOfVertices{static_cast<int>(indices.size())}
{
}

Shape::Shape(Shape && rhs) noexcept
    : objectIds{rhs.objectIds}
    , numOfVertices{rhs.numOfVertices}
{
    rhs.numOfVertices = 0;
}

auto Shape::operator = (Shape && rhs) noexcept
    -> Shape &
{
    objectIds = rhs.objectIds;

    numOfVertices = rhs.numOfVertices;

    rhs.numOfVertices = 0;

    return *this;
}

Shape::~Shape()
{
    if (numOfVertices != 0)
    {
        glDeleteVertexArrays(1, &objectIds.vertexArrayObjectId);

        glDeleteVertexArrays(1, &objectIds.elementBufferObjectId);

        glDeleteVertexArrays(1, &objectIds.vertexBufferObjectId);
    }
}

auto Shape::draw() const
    -> void
{
    // Culling is not appropriate for all shapes. This should be done conditionally in the future.
    // However, when appropriate, it will save at least 50% of fragment shader calls.
    glEnable(GL_CULL_FACE);

    glBindVertexArray(objectIds.vertexArrayObjectId);

    glDrawElements(GL_TRIANGLES, numOfVertices, GL_UNSIGNED_INT, 0);
    
    glBindVertexArray(0);
}

} // namespace ape
