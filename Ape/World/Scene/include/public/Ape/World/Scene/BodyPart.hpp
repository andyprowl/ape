#pragma once

#include <Ape/World/Scene/BodyPartMesh.hpp>

#include <glm/mat4x4.hpp>

#include <vector>

namespace ape
{

class Body;
class ModelPart;

class BodyPart
{

public:

    BodyPart(Body & body, ModelPart const & part);

    auto getBody() const
        -> Body const &;

    auto getModel() const
        -> const ModelPart &;

    auto getParent() const
        -> const BodyPart *;

    auto getNumOfComponents() const
        -> int;

    auto getComponent(int index)
        -> BodyPart &;

    auto getMeshes() const
        -> std::vector<BodyPartMesh> const &;

    auto getLocalTransformation() const
        -> glm::mat4 const &;

    auto setLocalTransformation(glm::mat4 const & newTransformation)
        -> void;

    auto getWorldTransformation() const
        -> glm::mat4 const &;

    auto getWorldNormalTransformation() const
        -> glm::mat3 const &;

    // Implemented as a member because it does not require updating the normal matrix
    auto scaleUniformly(float factor)
        -> void;

    // Implemented as a member because it does not require updating the normal matrix
    auto translate(glm::vec3 const & offset)
        -> void;

private:

    friend class Body;

private:

    auto setBody(Body & newBody)
        -> void;

    auto getParentWorldTransformation() const
        -> glm::mat4;

    auto onParentTransformationChanged(glm::mat4 const & newTransformation)
        -> void;

    auto updateWorldTransformation()
        -> void;

    auto updateWorldNormalTransformation()
        -> void;

    auto updateDescendentWorldTransformations()
        -> void;

    auto makeMeshes() const
        -> std::vector<BodyPartMesh>;

private:

    Body * body;

    ModelPart const * part;

    glm::mat4 localTransformation;

    glm::mat4 worldTransformation;

    glm::mat3 worldNormalTransformation;

    std::vector<BodyPartMesh> meshes;

};

auto isRoot(BodyPart const & part)
    -> bool;

auto getLocalPosition(BodyPart const & part)
    -> glm::vec3;

auto setLocalPosition(BodyPart & part, glm::vec3 const & newPosition)
    -> void;

auto getGlobalPosition(BodyPart const & part)
    -> glm::vec3;

} // namespace ape
