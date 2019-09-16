#pragma once

#include "Attenuation.hpp"
#include "Light.hpp"

#include <glm/gtc/matrix_transform.hpp>

class PointLight : public Light
{

public:

    PointLight(glm::vec3 const & position, Attenuation const & attenuation, Color const & color)
        : Light{color}
        , position{position}
        , attenuation{attenuation}
    {
    }

public:

    glm::vec3 position;

    Attenuation attenuation;

};