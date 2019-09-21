#pragma once

#include <glm/mat4x4.hpp>

#include <vector>

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

    auto getLocalTransformation() const
        -> glm::mat4 const &;

    auto setLocalTransformation(glm::mat4 const & newTransformation)
        -> void;

    auto getGlobalTransformation() const
        -> glm::mat4 const &;

    auto getGlobalNormalTransformation() const
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

    auto getParentGlobalTransformation() const
        -> glm::mat4;

    auto onParentTransformationChanged(glm::mat4 const & newTransformation)
        -> void;

    auto updateGlobalTransformation()
        -> void;

    auto updateGlobalNormalTransformation()
        -> void;

    auto updateDescendentGlobalTransformations()
        -> void;

private:

    Body * body;

    ModelPart const * part;

    glm::mat4 localTransformation;

    glm::mat4 globalTransformation;

    glm::mat3 globalNormalTransformation;

};

auto isRoot(BodyPart const & part)
    -> bool;

auto getLocalPosition(BodyPart const & part)
    -> glm::vec3;

auto setLocalPosition(BodyPart & part, glm::vec3 const & newPosition)
    -> void;

auto getGlobalPosition(BodyPart const & part)
    -> glm::vec3;
