#pragma once

#include <glm/mat4x4.hpp>

#include <vector>

class ModelPart;

class BodyPart
{

public:

    explicit BodyPart(ModelPart const & part);

    BodyPart(ModelPart const & part, BodyPart const & parent);

    auto getModel() const
        -> const ModelPart &;

    auto getComponents() const
        -> std::vector<BodyPart *> const &;

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

    BodyPart(ModelPart const & part, BodyPart const * parent);

    auto getParentGlobalTransformation() const
        -> glm::mat4;

    auto registerComponent(BodyPart & component)
        -> void;

    auto onParentTransformationChanged(glm::mat4 const & newTransformation)
        -> void;

    auto updateGlobalTransformation()
        -> void;

    auto updateGlobalNormalTransformation()
        -> void;

    auto updateDescendentGlobalTransformations() const
        -> void;

private:

    ModelPart const * part;
    
    BodyPart const * parent;

    std::vector<BodyPart *> components;

    glm::mat4 localTransformation;

    glm::mat4 globalTransformation;

    glm::mat3 globalNormalTransformation;

};

auto getLocalPosition(BodyPart const & part)
    -> glm::vec3;

auto setLocalPosition(BodyPart & part, glm::vec3 const & newPosition)
    -> void;

auto getGlobalPosition(BodyPart const & part)
    -> glm::vec3;
