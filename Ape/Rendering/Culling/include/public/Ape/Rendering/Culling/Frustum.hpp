#pragma once

#include <Ape/Rendering/Culling/Hexahedron.hpp>
#include <Ape/Rendering/Culling/Plane.hpp>

#include <array>
#include <utility>

namespace ape
{

class Frustum : public Hexahedron
{

public:

    Frustum(
        Plane const & right,
        Plane const & left,
        Plane const & top,
        Plane const & bottom,
        Plane const & front,
        Plane const & back);

    auto getPlane(Face const face) const
        -> Plane const &
    {
        return planes[static_cast<std::size_t>(face)];
    }

    auto getPlanes() const
        -> std::array<Plane, 6u> const &
    {
        return planes;
    }

    auto getVertex(Vertex const vertex) const
        -> glm::vec3 const &
    {
        return vertices[static_cast<std::size_t>(vertex)];
    }

    auto getVertices() const
        -> std::array<glm::vec3, 8u> const &
    {
        return vertices;
    }

    auto getEdge(Face const f1, Face const f2) const
        -> std::pair<glm::vec3, glm::vec3>
    {
        auto const edge = ape::getEdge(f1, f2);

        return {getVertex(edge.first), getVertex(edge.second)};
    }

private:

    auto makeVertices() const
        -> std::array<glm::vec3, 8u>;

    auto calculatePlaneIntersection(Face f1, Face f2, Face f3) const
        -> glm::vec3;

private:

    std::array<Plane, 6u> planes;

    std::array<glm::vec3, 8u> vertices;

};

} // namespace ape
