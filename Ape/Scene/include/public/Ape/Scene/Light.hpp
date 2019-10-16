#pragma once

#include <Foundational/Signal/Signal.hpp>

#include <glm/vec3.hpp>

namespace ape
{

class ShaderProgram;

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

    Light(Color const & color, bool isTurnedOn);

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

public:

    mutable Signal<auto () -> void> onLightChanged;

private:

    Color color;

    bool isOn;

};

} // namespace ape
