#include <Ape/Rendering/Culling/Frustum.hpp>

namespace ape
{

namespace
{

// See the formula at http://geomalgorithms.com/a05-_intersect-1.html
auto calculatePlaneIntersection(Plane const & p1, Plane const & p2, Plane const & p3)
    -> glm::vec3
{
    const auto num = 
        -p1.offset * glm::cross(p2.normal, p3.normal) +
        -p2.offset * glm::cross(p3.normal, p1.normal) +
        -p3.offset * glm::cross(p1.normal, p2.normal);

    const auto denom = glm::dot(p1.normal, glm::cross(p2.normal, p3.normal));

    return num / denom;
}

} // unnamed namespace

Frustum::Frustum(
    Plane const & right,
    Plane const & left,
    Plane const & top,
    Plane const & bottom,
    Plane const & front,
    Plane const & back)
    : planes{{right, left, top, bottom, front, back}}
    , vertices{makeVertices()}
{
}

auto Frustum::makeVertices() const
    -> std::array<glm::vec3, 8u>
{
    return {{
        calculatePlaneIntersection(Face::right, Face::top, Face::front),
        calculatePlaneIntersection(Face::right, Face::top, Face::back),
        calculatePlaneIntersection(Face::right, Face::bottom, Face::front),
        calculatePlaneIntersection(Face::right, Face::bottom, Face::back),
        calculatePlaneIntersection(Face::left, Face::top, Face::front),
        calculatePlaneIntersection(Face::left, Face::top, Face::back),
        calculatePlaneIntersection(Face::left, Face::bottom, Face::front),
        calculatePlaneIntersection(Face::left, Face::bottom, Face::back)}};
}

auto Frustum::calculatePlaneIntersection(Face const f1, Face const f2, Face const f3) const
    -> glm::vec3
{
    return ape::calculatePlaneIntersection(getPlane(f1), getPlane(f2), getPlane(f3));
}

} // namespace ape
