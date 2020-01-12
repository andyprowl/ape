#pragma once

#include <Ape/World/Shape/ShapeBounds.hpp>

namespace ape
{

class BodyPart;
class Mesh;

class BodyPartMesh
{

public:

    BodyPartMesh(BodyPart const & part, Mesh const & model, glm::mat4 const & worldTransformation);

    auto getPart() const
        -> BodyPart const &;
    
    auto getModel() const
        -> Mesh const &;

    auto getBoundingVolumes() const
        -> ShapeBounds const &;

private:

    friend class BodyPart;

    auto onPartTransformationChanged(glm::mat4 const & worldTransformation)
        -> void;

private:

    const BodyPart * part;

    const Mesh * model;

    ShapeBounds boundingVolumes;

};

} // namespace ape
