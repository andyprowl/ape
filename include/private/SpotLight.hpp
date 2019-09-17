#pragma once

#include "Attenuation.hpp"
#include "Light.hpp"

class SpotLight : public Light
{

public:

    class CutoffAngle
    {

    public:

        CutoffAngle(float const inner, float const outer)
            : inner{inner}
            , outer{outer}
        {
        }
    
    public:
    
        float inner;
        
        float outer;

    };

public:

    SpotLight(
        glm::vec3 const & position,
        glm::vec3 const & direction,
        CutoffAngle const & cutoff,
        Attenuation const & attenuation,
        Color const & color,
        bool const isTurnedOn)
        : Light{color, isTurnedOn}
        , position{position}
        , direction{direction}
        , cutoff{cutoff}
        , attenuation{attenuation}
    {
    }
    
public:

    glm::vec3 position;

    glm::vec3 direction;

    CutoffAngle cutoff;

    Attenuation attenuation;

};