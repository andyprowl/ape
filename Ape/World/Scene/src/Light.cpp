#include <Ape/World/Scene/Light.hpp>

namespace ape
{

Light::Light(std::string name, Color const & color, bool const isTurnedOn)
    : name{std::move(name)}
    , color{color}
    , isOn{isTurnedOn}
{
}

auto Light::getName() const
    -> std::string_view
{
    return name;
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
