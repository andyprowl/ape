#include "square.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <array>
#include <vector>

class SquareBuilder
{

public:

    using Position = glm::vec3;

    using Normal = glm::vec3;

    using TextureCoordinates = glm::vec2;

    class Vertex
    {

    public:

        Position position;
        
        Normal normal;
        
        TextureCoordinates textureCoords;

    };

    using Face = std::array<Vertex, 4u>;

public:

    SquareBuilder()
    {
        auto const numOfFaces = 6;

        auto const numOfVerticesPerFace = 4;

        auto const numOfFloatsPerVertex = 3 + 3 + 2; // position, normal, textureCoords
        
        vertices.reserve(numOfFaces * numOfVerticesPerFace * numOfFloatsPerVertex);
    }

    auto build(SquareNormalDirection const normalDirection)
        -> Shape
    {
        auto const face = getBottomFace(normalDirection);

        // bottom
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

    static auto getBottomFace(SquareNormalDirection const normalDirection)
        -> Face
    {
        auto const normal = getBottomFaceNormal(normalDirection);

        return {{
            {Position{-0.5f, 0.0f, -0.5f}, normal, TextureCoordinates{0.0f, 0.0f}},
            {Position{0.5f, 0.0f, -0.5f}, normal, TextureCoordinates{1.0f, 0.0f}},
            {Position{0.5f, 0.0f, 0.5f}, normal, TextureCoordinates{1.0f, 1.0f}},
            {Position{-0.5f, 0.0f, 0.5f}, normal, TextureCoordinates{0.0f, 1.0f}}}};
    }

    static auto getBottomFaceNormal(SquareNormalDirection const normalDirection)
        -> Normal
    {
        auto const outboundNormal = glm::vec3{0.0f, -1.0f, 0.0f};

        auto const isOutbound = (normalDirection == SquareNormalDirection::outbound);

        auto const normalModifier = isOutbound ? 1.0f : -1.0f;

        return (normalModifier * outboundNormal);
    }
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
        auto const stride = 8;

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

auto makeSquare(SquareNormalDirection const normalDirection)
    -> Shape
{
    return SquareBuilder{}.build(normalDirection);
}
