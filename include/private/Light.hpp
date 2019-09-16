#pragma once

#include "Transformable.hpp"

#include <glm/vec3.hpp>

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

    Light(Color const & color)
        : color{color}
    {
    }

public:

    Color color;

};
