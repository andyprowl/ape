#pragma once

#include <Ape/Rendering/Culling/ContainmentRelation.hpp>

#include <glm/vec3.hpp>

namespace ape
{

class Sphere;

class Culler
{

public:

    virtual ~Culler() = default;

    virtual auto isSphereContained(Sphere const & sphere) const
        -> ContainmentRelation = 0;

    virtual auto isPointContained(glm::vec3 const & point) const
        -> bool = 0;

};

} // namespace ape
