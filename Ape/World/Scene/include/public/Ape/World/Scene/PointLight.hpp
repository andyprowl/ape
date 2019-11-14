#pragma once

#include <Ape/World/Scene/Attenuation.hpp>
#include <Ape/World/Scene/Light.hpp>

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
        bool isTurnedOn);

    auto getPosition() const
        -> glm::vec3;

    auto setPosition(glm::vec3 const & newPosition)
        -> void;

    auto getAttenuation() const
        -> Attenuation const &;

    auto setAttenuation(Attenuation const & newAttenuation)
        -> void;

private:

    glm::vec3 position;

    Attenuation attenuation;

};

} // namespace ape
