#pragma once

#include <Scene/Attenuation.hpp>
#include <Scene/Light.hpp>

#include <Signal/Signal.hpp>

namespace ape
{

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
        bool const isTurnedOn);

    auto getPosition() const
        -> glm::vec3;

    auto setPosition(glm::vec3 const & newPosition)
        -> void;
    
    auto getDirection() const
        -> glm::vec3;

    auto setDirection(glm::vec3 const & newDirection)
        -> void;

    auto getCutoff() const
        -> CutoffAngle const &;

    auto setCutoff(CutoffAngle const & newCutoff)
        -> void;

    auto getAttenuation() const
        -> Attenuation const &;

    auto setAttenuation(Attenuation const & newAttenuation)
        -> void;

    auto setPlacement(glm::vec3 const & newPosition, glm::vec3 const & newDirection)
        -> void;

private:

    glm::vec3 position;

    glm::vec3 direction;

    CutoffAngle cutoff;

    Attenuation attenuation;

};

} // namespace ape
