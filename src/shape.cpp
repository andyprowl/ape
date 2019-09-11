#include "shape.hpp"

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

class SquareBuilder
{

public:

    using Vertex = std::tuple<glm::vec3, glm::vec3, glm::vec2>;

    using Face = std::array<Vertex, 4u>;

public:

    SquareBuilder()
    {
        vertices.reserve(6 * 4 * (3 + 3 + 2));
    }

    auto build(glm::vec3 const & color)
        -> Shape
    {
        auto face = Face{{
            Vertex{{-0.5f, -0.5f, -0.5f}, color, {0.0f, 0.0f}},
            Vertex{{0.5f, -0.5f, -0.5f}, color, {1.0f, 0.0f}},
            Vertex{{0.5f, -0.5f, 0.5f}, color, {1.0f, 1.0f}},
            Vertex{{-0.5f, -0.5f, 0.5f}, color, {0.0f, 1.0f}}}};

        addFace(face, glm::mat4{1.0f});

        addFace(face, translate({0.0f, 1.0f, 0.0f}));

        addFace(face, rotate(-90.0f, {1.0f, 0.0f, 0.0f}));

        addFace(face, translate({0.0f, 0.0f, -1.0f}) * rotate(-90.0f, {1.0f, 0.0f, 0.0f}));

        addFace(face, rotate(-90.0f, {0.0f, 1.0f, 0.0f}) * rotate(-90.0f, {1.0f, 0.0f, 0.0f}));

        addFace(
            face,
            translate({1.0f, 0.0f, 0.0f}) *
            rotate(-90.0f, {0.0f, 1.0f, 0.0f}) *
            rotate(-90.0f, {1.0f, 0.0f, 0.0f}));

        return Shape{std::move(vertices), std::move(indices)};
    }

private:

    static auto rotate(float const degrees, glm::vec3 const & axis)
        -> glm::mat4
    {
        return glm::rotate(glm::mat4{1.0f}, glm::radians(degrees), axis);
    }

    static auto translate(glm::vec3 const & offset)
        -> glm::mat4
    {
        return glm::translate(glm::mat4{1.0f}, offset);
    }

    auto addFace(Face const & face, glm::mat4 const & transformation)
        -> void
    {
        for (auto const & v : face)
        {
            auto const tv = transformation * glm::vec4{std::get<0>(v), 1.0f};

            pushVertex(glm::vec3{transformation * glm::vec4{std::get<0>(v), 1.0f}});

            pushVertex(std::get<1>(v));

            pushVertex(std::get<2>(v));
        }

        pushFaceIndices();
    }

    auto pushVertex(glm::vec3 const & v)
        -> void
    {
        vertices.push_back(v.x);

        vertices.push_back(v.y);

        vertices.push_back(v.z);
    }

    auto pushVertex(glm::vec2 const & v)
        -> void
    {
        vertices.push_back(v.x);

        vertices.push_back(v.y);
    }

    auto pushFaceIndices()
        -> void
    {
        auto const base = (vertices.size() / 8) - 4;

        indices.push_back(base + 0);

        indices.push_back(base + 1);

        indices.push_back(base + 3);

        indices.push_back(base + 1);

        indices.push_back(base + 2);

        indices.push_back(base + 3);
    }

private:

    std::vector<float> vertices;

    std::vector<unsigned int> indices;

};

auto makeSquare(glm::vec3 const & color)
    -> Shape
{
    return SquareBuilder{}.build(color);
}
