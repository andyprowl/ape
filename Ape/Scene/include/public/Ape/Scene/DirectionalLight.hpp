#pragma once

#include <Scene/Light.hpp>

#include <Signal/Signal.hpp>

namespace ape
{

class DirectionalLight : public Light
{

public:

    DirectionalLight(glm::vec3 const & direction, Color const & color, bool isTurnedOn);

    auto getDirection() const
        -> glm::vec3;

    auto setDirection(glm::vec3 const & newDirection)
        -> void;

private:

    glm::vec3 direction;

};

} // namespace ape
