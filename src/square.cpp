#include "square.h"

#include <glm/gtc/matrix_transform.hpp>

#include <array>
#include <vector>

class SquareBuilder
{

public:

    class Vertex
    {

    public:

        glm::vec3 position;
        
        glm::vec3 normal;
        
        glm::vec3 color;
        
        glm::vec2 textureCoords;

    };

    using Face = std::array<Vertex, 4u>;

public:

    SquareBuilder()
    {
        vertices.reserve(6 * 4 * (3 + 3 + 2));
    }

    auto build(glm::vec3 const & color)
        -> Shape
    {
        auto const face = Face{{
            Vertex{{-0.5f, 0.0f, -0.5f}, {0.0f, -1.0f, 0.0f}, color, {0.0f, 0.0f}},
            Vertex{{0.5f, 0.0f, -0.5f}, {0.0f, -1.0f, 0.0f}, color, {1.0f, 0.0f}},
            Vertex{{0.5f, 0.0f, 0.5f}, {0.0f, -1.0f, 0.0f}, color, {1.0f, 1.0f}},
            Vertex{{-0.5f, 0.0f, 0.5f}, {0.0f, -1.0f, 0.0f}, color, {0.0f, 1.0f}}}};

        // low
        addFace(face, translate({0.0f, -0.5f, 0.0f}));

        // top
        addFace(face, translate({0.0f, 0.5f, 0.0f}) * rotate(180.0f, {1.0f, 0.0f, 0.0f}));

        // front
        addFace(face, translate({0.0f, 0.0f, 0.5f}) * rotate(-90.0f, {1.0f, 0.0f, 0.0f}));

        // back
        addFace(face, translate({0.0f, 0.0f, -0.5f}) * rotate(90.0f, {1.0f, 0.0f, 0.0f}));

        // left
        addFace(face, translate({-0.5f, 0.0f, 0.0f}) * rotate(-90.0f, {0.0f, 0.0f, 1.0f}));

        // right
        addFace(face, translate({0.5f, 0.0f, 0.0f}) * rotate(90.0f, {0.0f, 0.0f, 1.0f}));

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

    static auto transform(glm::vec3 const & v, glm::mat4 const & transformation)
        -> glm::vec3
    {
        return glm::vec3{transformation * glm::vec4{v, 1.0f}};
    }

    auto addFace(Face const & face, glm::mat4 const & transformation)
        -> void
    {
        for (auto const & v : face)
        {
            pushVertex(transform(v.position, transformation));

            pushVertex(glm::normalize(transform(v.normal, transformation)));

            pushVertex(v.color);

            pushVertex(v.textureCoords);
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
        auto const stride = 11;

        auto const base = (vertices.size() / stride) - 4;

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
