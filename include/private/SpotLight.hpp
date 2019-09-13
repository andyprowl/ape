#pragma once

#include "Light.hpp"

class SpotLight : public Light
{

public:

    SpotLight(
        glm::vec3 const & position,
        glm::vec3 const & direction,
        float const innerCutoffAngle,
        float const outerCutoffAngle,
        Color const & color)
        : Light{color}
        , position{position}
        , direction{direction}
        , innerCutoffAngle{innerCutoffAngle}
        , outerCutoffAngle{outerCutoffAngle}
    {
    }
    
public:

    glm::vec3 position;

    glm::vec3 direction;

    float innerCutoffAngle;

    float outerCutoffAngle;

};