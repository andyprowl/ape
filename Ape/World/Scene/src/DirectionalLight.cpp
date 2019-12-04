#include <Ape/World/Scene/DirectionalLight.hpp>

namespace ape
{

DirectionalLight::DirectionalLight(
    std::string name,
    glm::vec3 const & direction,
    Color const & color,
    bool const isTurnedOn)
    : Light{std::move(name), color, isTurnedOn}
    , direction{direction}
{
}

auto DirectionalLight::getDirection() const
    -> glm::vec3
{
    return direction;
}

auto DirectionalLight::setDirection(glm::vec3 const & newDirection)
    -> void
{
    direction = newDirection;

    onLightChanged.fire();
}

} // namespace ape
