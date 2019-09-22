#pragma once

#include <Ape/Light.hpp>

namespace ape
{

class DirectionalLight : public Light
{

public:

    DirectionalLight(glm::vec3 const & direction, Color const & color, bool const isTurnedOn)
        : Light{color, isTurnedOn}
        , direction{direction}
    {
    }
    
public:

    glm::vec3 direction;

};

} // namespace ape
