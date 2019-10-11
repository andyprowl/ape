#include <Scene/SpotLight.hpp>

namespace ape
{

SpotLight::SpotLight(
    glm::vec3 const & position,
    glm::vec3 const & direction,
    CutoffAngle const & cutoff,
    Attenuation const & attenuation,
    Color const & color,
    bool const isTurnedOn)
    : Light{color, isTurnedOn}
    , position{position}
    , direction{direction}
    , cutoff{cutoff}
    , attenuation{attenuation}
{
}

auto SpotLight::getPosition() const
    -> glm::vec3
{
    return position;
}

auto SpotLight::setPosition(glm::vec3 const & newPosition)
    -> void
{
    position = newPosition;

    onLightChanged.fire();
}

auto SpotLight::getDirection() const
    -> glm::vec3
{
    return direction;
}

auto SpotLight::setDirection(glm::vec3 const & newDirection)
    -> void
{
    direction = newDirection;

    onLightChanged.fire();
}

auto SpotLight::getCutoff() const
    -> CutoffAngle const &
{
    return cutoff;
}

auto SpotLight::setCutoff(CutoffAngle const & newCutoff)
    -> void
{
    cutoff = newCutoff;

    onLightChanged.fire();
}

auto SpotLight::getAttenuation() const
    -> Attenuation const &
{
    return attenuation;
}

auto SpotLight::setAttenuation(Attenuation const & newAttenuation)
    -> void
{
    attenuation = newAttenuation;

    onLightChanged.fire();
}

auto SpotLight::setPlacement(glm::vec3 const & newPosition, glm::vec3 const & newDirection)
    -> void
{
    position = newPosition;

    direction = newDirection;

    onLightChanged.fire();
}

} // namespace ape
