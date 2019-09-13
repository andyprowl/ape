#include "Shape.hpp"

#include "glad/glad.h"

#include <glm/gtc/matrix_transform.hpp>

#include <algorithm>
#include <array>
#include <numeric>
#include <tuple>

namespace
{

auto encodeFloatOffset(int const numOfFloats)
    -> void *
{
    return reinterpret_cast<void *>(numOfFloats * sizeof(float));
};

auto makeVertexBufferObject(std::vector<float> const & vertices)
    -> unsigned int
{
    auto vboId = static_cast<unsigned int>(0);

    glGenBuffers(1, &vboId);

    glBindBuffer(GL_ARRAY_BUFFER, vboId);

    glBufferData(
        GL_ARRAY_BUFFER,
        vertices.size() * sizeof(float),
        vertices.data(),
        GL_STATIC_DRAW);

    auto const stride = 8 * sizeof(float);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, encodeFloatOffset(0));

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, encodeFloatOffset(3));

    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, encodeFloatOffset(6));

    glEnableVertexAttribArray(2);

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
        indices.size() * sizeof(float),
        indices.data(),
        GL_STATIC_DRAW);

    return eboId;
}

auto makeVertices(
    std::vector<float> const & vertices,
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

Shape::Shape(std::vector<float> const & vertices, std::vector<unsigned int> const & indices)
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
    glBindVertexArray(objectIds.vertexArrayObjectId);

    glDrawElements(GL_TRIANGLES, numOfVertices, GL_UNSIGNED_INT, 0);
    
    glBindVertexArray(0);
}
