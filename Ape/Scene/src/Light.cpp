#include <Scene/Light.hpp>

namespace ape
{

Light::Light(Color const & color, bool const isTurnedOn)
    : color{color}
    , isOn{isTurnedOn}
{
}

auto Light::getColor() const
    -> Color
{
    return color;
}

auto Light::setColor(Color newColor)
    -> void
{
    color = newColor;

    onLightChanged.fire();
}

auto Light::isTurnedOn() const
    -> bool
{
    return isOn;
}

auto Light::turnOn()
    -> void
{
    isOn = true;

    onLightChanged.fire();
}

auto Light::turnOff()
    -> void
{
    isOn = false;

    onLightChanged.fire();
}

auto Light::toggle()
    -> void
{
    isOn = !isOn;

    onLightChanged.fire();
}

} // namespace ape
