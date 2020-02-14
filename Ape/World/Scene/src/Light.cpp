#include <Ape/World/Scene/Light.hpp>

namespace ape
{

Light::Light(std::string name, Color const & color, bool const isTurnedOn, bool const castsShadow)
    : name{std::move(name)}
    , color{color}
    , isOn{isTurnedOn}
    , castsShadow{castsShadow}
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
    if (isOn)
    {
        return;
    }

    isOn = true;

    onLightChanged.fire();
}

auto Light::turnOff()
    -> void
{
    if (not isOn)
    {
        return;
    }

    isOn = false;

    onLightChanged.fire();
}

auto Light::toggle()
    -> void
{
    isOn = !isOn;

    onLightChanged.fire();
}

auto Light::isCastingShadow() const
    -> bool
{
    return castsShadow;
}

auto Light::enableShadowCasting()
    -> void
{
    if (castsShadow)
    {
        return;
    }

    castsShadow = true;

    onLightChanged.fire();
}

auto Light::disableShadowCasting()
    -> void
{
    if (not castsShadow)
    {
        return;
    }

    castsShadow = false;

    onLightChanged.fire();
}

auto Light::toggleShadowCasting()
    -> void
{
    castsShadow = !castsShadow;

    onLightChanged.fire();
}

} // namespace ape
