#pragma once

#include <Core/Attenuation.hpp>
#include <Core/Light.hpp>

#include <glm/gtc/matrix_transform.hpp>

namespace ape
{

class PointLight : public Light
{

public:

    PointLight(
        glm::vec3 const & position,
        Attenuation const & attenuation,
        Color const & color,
        bool const isTurnedOn)
        : Light{color, isTurnedOn}
        , position{position}
        , attenuation{attenuation}
    {
    }

public:

    glm::vec3 position;

    Attenuation attenuation;

};

} // namespace ape