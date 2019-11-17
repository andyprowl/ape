#include <Ape/World/Shape/ShapeBounds.hpp>

#include <algorithm>

namespace ape
{

namespace
{

auto getMaxScalingFactor(glm::mat4 const & transformation)
    -> float
{
    auto const scaling = glm::vec3{
        glm::length(transformation[0]),
        glm::length(transformation[1]),
        glm::length(transformation[2])};

    return std::max({scaling.x, scaling.y, scaling.z});
}

} // unnamed namespace

auto ShapeBounds::transform(glm::mat4 const & transformation)
    -> void
{
    box.transform(transformation);

    auto const center = box.getCenter();

    sphere.setCenter(center);

    auto const scaling = getMaxScalingFactor(transformation);

    sphere.scale(scaling);
}

auto ShapeBounds::translate(glm::vec3 const & offset)
    -> void
{
    box.translate(offset);

    sphere.translate(offset);
}

auto ShapeBounds::scale(float const factor)
    -> void
{
    box.scale(factor);

    sphere.scale(factor);
}

} // namespace ape
