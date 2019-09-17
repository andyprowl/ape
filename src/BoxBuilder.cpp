#include "BoxBuilder.hpp"
#include "Vertex.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <array>
#include <vector>

namespace
{

auto rotate(float const degrees, glm::vec3 const & axis)
    -> glm::mat4
{
    return glm::rotate(glm::mat4{1.0f}, glm::radians(degrees), axis);
}

auto scale(glm::vec3 const & factor)
    -> glm::mat4
{
    return glm::scale(glm::mat4{1.0f}, factor);
}

auto translate(glm::vec3 const & offset)
    -> glm::mat4
{
    return glm::translate(glm::mat4{1.0f}, offset);
}

auto transform(glm::vec3 const & v, glm::mat4 const & transformation)
    -> glm::vec3
{
    return glm::vec3{transformation * glm::vec4{v, 1.0f}};
}

class StatefulBuilder
{

    using Size = BoxBuilder::Size;

    using Position = BoxBuilder::Position;

    using Face = std::array<Vertex, 4u>;

    static int const numOfFaces = 6;

    static int const numOfVerticesPerFace = 4;

public:

    StatefulBuilder(
        NormalDirection const normalDirection,
        Size const & size,
        Position const & center)
        : normalDirection{normalDirection}
        , size{size}
        , center{center}
    {
        vertices.reserve(numOfFaces * numOfVerticesPerFace);
    }

    auto build()
        -> Shape
    {
        auto const face = getBottomFace();

        // bottom
        addScaledFace(face, translate({0.0f, -0.5f, 0.0f}));

        // top
        addScaledFace(face, translate({0.0f, 0.5f, 0.0f}) * rotate(180.0f, {1.0f, 0.0f, 0.0f}));

        // front
        addScaledFace(face, translate({0.0f, 0.0f, 0.5f}) * rotate(-90.0f, {1.0f, 0.0f, 0.0f}));

        // back
        addScaledFace(face, translate({0.0f, 0.0f, -0.5f}) * rotate(90.0f, {1.0f, 0.0f, 0.0f}));

        // left
        addScaledFace(face, translate({-0.5f, 0.0f, 0.0f}) * rotate(-90.0f, {0.0f, 0.0f, 1.0f}));

        // right
        addScaledFace(face, translate({0.5f, 0.0f, 0.0f}) * rotate(90.0f, {0.0f, 0.0f, 1.0f}));

        return Shape{std::move(vertices), std::move(indices)};
    }

private:

    auto getBottomFace() const
        -> Face
    {
        auto const normal = getBottomFaceNormal();
        
        return {{
            {center + Position{-0.5f, 0.0f, -0.5f}, normal, {0.0f, 0.0f}},
            {center + Position{0.5f, 0.0f, -0.5f}, normal, {1.0f, 0.0f}},
            {center + Position{0.5f, 0.0f, 0.5f}, normal, {1.0f, 1.0f}},
            {center + Position{-0.5f, 0.0f, 0.5f}, normal, {0.0f, 1.0f}}}};
    }

    auto getBottomFaceNormal() const
        -> glm::vec3
    {
        auto const outboundNormal = glm::vec3{0.0f, -1.0f, 0.0f};

        auto const isOutbound = (normalDirection == NormalDirection::outbound);

        auto const normalModifier = isOutbound ? 1.0f : -1.0f;

        return (normalModifier * outboundNormal);
    }

    auto addScaledFace(Face const & face, glm::mat4 const & transformation)
        -> void
    {
        auto scaling = scale(size);

        for (auto const & v : face)
        {
            auto const position = transform(v.position, scaling * transformation);

            auto const normal = glm::normalize(transform(v.normal, transformation));

            vertices.emplace_back(position, normal, v.textureCoordinates);
        }

        pushFaceIndices();
    }

    auto pushFaceIndices()
        -> void
    {
        auto const firstFaceVertexIndex = 
            static_cast<unsigned int>(vertices.size() - numOfVerticesPerFace);

        indices.push_back(firstFaceVertexIndex + 0);

        indices.push_back(firstFaceVertexIndex + 1);

        indices.push_back(firstFaceVertexIndex + 3);

        indices.push_back(firstFaceVertexIndex + 1);

        indices.push_back(firstFaceVertexIndex + 2);

        indices.push_back(firstFaceVertexIndex + 3);
    }

private:

    NormalDirection normalDirection;

    Size size;

    Position center;

    std::vector<Vertex> vertices;

    std::vector<unsigned int> indices;

};

} // unnamed namespace

auto BoxBuilder::build(
    NormalDirection const normalDirection,
    Size const & size,
    Position const & center) const
    -> Shape
{
    return StatefulBuilder{normalDirection, size, center}.build();
}
