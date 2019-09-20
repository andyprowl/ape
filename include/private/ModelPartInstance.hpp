#pragma once

#include <glm/mat4x4.hpp>

#include <vector>

class ModelPart;

class ModelPartInstance
{

public:

    explicit ModelPartInstance(ModelPart const & part);

    ModelPartInstance(ModelPart const & part, ModelPartInstance const & parent);

    auto getModel() const
        -> const ModelPart &;

    auto getComponents() const
        -> std::vector<ModelPartInstance *> const &;

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

    ModelPartInstance(ModelPart const & part, ModelPartInstance const * parent);

    auto getParentGlobalTransformation() const
        -> glm::mat4;

    auto registerComponent(ModelPartInstance & component)
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
    
    ModelPartInstance const * parent;

    std::vector<ModelPartInstance *> components;

    glm::mat4 localTransformation;

    glm::mat4 globalTransformation;

    glm::mat3 globalNormalTransformation;

};

auto getLocalPosition(ModelPartInstance const & part)
    -> glm::vec3;

auto setLocalPosition(ModelPartInstance & part, glm::vec3 const & newPosition)
    -> void;

auto getGlobalPosition(ModelPartInstance const & part)
    -> glm::vec3;
