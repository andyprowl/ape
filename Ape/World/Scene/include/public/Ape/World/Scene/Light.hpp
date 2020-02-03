#pragma once

#include <Basix/Signal/Signal.hpp>

#include <glm/vec3.hpp>

#include <string>
#include <string_view>

namespace ape
{

class Light
{

public:

    class Color
    {

    public:
    
        Color(
            glm::vec3 const & ambient,
            glm::vec3 const & diffuse,
            glm::vec3 const & specular)
            : ambient{ambient}
            , diffuse{diffuse}
            , specular{specular}
        {
        }

    public:
    
        glm::vec3 ambient;
        
        glm::vec3 diffuse;
        
        glm::vec3 specular;

    };

public:

    Light(std::string name, Color const & color, bool isTurnedOn, bool castsShadow);

    auto getName() const
        -> std::string_view;

    auto getColor() const
        -> Color;

    auto setColor(Color newColor)
        -> void;

    auto isTurnedOn() const
        -> bool;

    auto turnOn()
        -> void;

    auto turnOff()
        -> void;

    auto toggle()
        -> void;

    auto isCastingShadow() const
        -> bool;

    auto enableShadowCasting()
        -> void;

    auto disableShadowCasting()
        -> void;

    auto toggleShadowCasting()
        -> void;

public:

    mutable basix::Signal<auto () -> void> onLightChanged;

private:

    std::string name;

    Color color;

    bool isOn;

    bool castsShadow;

};

} // namespace ape
