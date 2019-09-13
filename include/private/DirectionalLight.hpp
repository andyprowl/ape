#pragma once

#include "Light.hpp"

class DirectionalLight : public Light
{

public:

    DirectionalLight(glm::vec3 const & direction, Color const & color)
        : Light{color}
        , direction{direction}
    {
    }
    
public:

    glm::vec3 direction;

};
