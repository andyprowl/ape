#pragma once

#include <Ape/World/Shape/ShapeBounds.hpp>

namespace ape
{

class Mesh;

class BodyPartMesh
{

public:

    BodyPartMesh(Mesh const & model, glm::mat4 const & worldTransformation);
    
    auto getModel() const
        -> Mesh const &;

    auto getBoundingVolumes() const
        -> ShapeBounds const &;

private:

    friend class BodyPart;

    auto onPartTransformationChanged(glm::mat4 const & worldTransformation)
        -> void;

private:

    const Mesh * model;

    ShapeBounds boundingVolumes;

};

} // namespace ape
