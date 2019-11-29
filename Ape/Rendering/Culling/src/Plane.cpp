#include <Ape/Rendering/Culling/Plane.hpp>

#include <glm/geometric.hpp>

namespace ape
{

namespace
{

auto isNormalized(Plane const & p)
    -> bool
{
    return (std::fabs(glm::length(p.normal) - 1.0f) <= 1.0e-6);
}

} // unnamed namespace

auto computeSignedDistance(glm::vec3 const & point, Plane const & plane)
    -> float
{
    assert(isNormalized(plane));

    return (glm::dot(plane.normal, point) + plane.offset);
}

} // namespace ape
