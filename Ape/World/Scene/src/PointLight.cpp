#include <Ape/World/Scene/PointLight.hpp>

namespace ape
{

PointLight::PointLight(
    std::string name,
    glm::vec3 const & position,
    Attenuation const & attenuation,
    Color const & color,
    bool const isTurnedOn)
    : Light{std::move(name), color, isTurnedOn}
    , position{position}
    , attenuation{attenuation}
{
}

auto PointLight::getPosition() const
    -> glm::vec3
{
    return position;
}

auto PointLight::setPosition(glm::vec3 const & newPosition)
    -> void
{
    position = newPosition;

    onLightChanged.fire();
}

auto PointLight::getAttenuation() const
    -> Attenuation const &
{
    return attenuation;
}

auto PointLight::setAttenuation(Attenuation const & newAttenuation)
    -> void
{
    attenuation = newAttenuation;

    onLightChanged.fire();
}

} // namespace ape
