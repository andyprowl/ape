#pragma once

#include "ModelPartInstance.hpp"

#include "ModelPart.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace
{

auto computeNormalTransformation(glm::mat4 const & transformation)
    -> glm::mat3
{
    return glm::mat3{glm::transpose(glm::inverse(transformation))};
}

} // unnamed namespace

ModelPartInstance::ModelPartInstance(ModelPart const & part)
    : ModelPartInstance{part, nullptr}
{
}

ModelPartInstance::ModelPartInstance(ModelPart const & part, ModelPartInstance const & parent)
    : ModelPartInstance{part, &parent}
{
}

auto ModelPartInstance::getComponents() const
    -> std::vector<ModelPartInstance *> const &
{
    return components;
}

auto ModelPartInstance::getModel() const
    -> const ModelPart &
{
    return *part;
}

auto ModelPartInstance::getLocalTransformation() const
    -> glm::mat4 const &
{
    return localTransformation;
}

auto ModelPartInstance::setLocalTransformation(glm::mat4 const & newTransformation)
    -> void
{
    localTransformation = newTransformation;

    updateGlobalTransformation();

    updateGlobalNormalTransformation();

    updateDescendentGlobalTransformations();
}

auto ModelPartInstance::getGlobalTransformation() const
    -> glm::mat4 const &
{
    return globalTransformation;
}

auto ModelPartInstance::getGlobalNormalTransformation() const
    -> glm::mat3 const &
{
    return globalNormalTransformation;
}

// Implemented as a member because it does not require updating the normal matrix
auto ModelPartInstance::scaleUniformly(float const factor)
    -> void
{
    localTransformation = glm::scale(localTransformation, {factor, factor, factor});

    updateGlobalTransformation();
}

// Implemented as a member because it does not require updating the normal matrix
auto ModelPartInstance::translate(glm::vec3 const & offset)
    -> void
{
    localTransformation = glm::translate(localTransformation, offset);

    updateGlobalTransformation();
}

ModelPartInstance::ModelPartInstance(ModelPart const & part, ModelPartInstance const * const parent)
    : part{&part}
    , parent{parent}
    , localTransformation{part.getTransformation()}
    , globalTransformation{getParentGlobalTransformation() * localTransformation}
    , globalNormalTransformation{computeNormalTransformation(globalTransformation)}
{
    components.reserve(part.getComponents().size());
}

auto ModelPartInstance::getParentGlobalTransformation() const
    -> glm::mat4
{
    if (parent != nullptr)
    {
        return parent->globalTransformation;
    }
    else
    {
        return glm::mat4{1.0f};
    }
}

auto ModelPartInstance::registerComponent(ModelPartInstance & component)
    -> void
{
    components.push_back(&component);
}

auto ModelPartInstance::onParentTransformationChanged(glm::mat4 const & newTransformation)
    -> void
{
    globalTransformation = newTransformation * localTransformation;

    updateGlobalNormalTransformation();

    updateDescendentGlobalTransformations();
}

auto ModelPartInstance::updateGlobalTransformation()
    -> void
{
    globalTransformation = getParentGlobalTransformation() * localTransformation;
}

auto ModelPartInstance::updateGlobalNormalTransformation()
    -> void
{
    globalNormalTransformation = computeNormalTransformation(globalTransformation);
}

auto ModelPartInstance::updateDescendentGlobalTransformations() const
    -> void
{
    for (auto component : components)
    {
        component->onParentTransformationChanged(globalNormalTransformation);
    }
}

auto getLocalPosition(ModelPartInstance const & part)
    -> glm::vec3
{
    auto const & transformation = part.getLocalTransformation();

    return transformation[3];
}

auto setLocalPosition(ModelPartInstance & part, glm::vec3 const & newPosition)
    -> void
{
    auto const offset = newPosition - getLocalPosition(part);

    part.translate(offset);
}

auto getGlobalPosition(ModelPartInstance const & part)
    -> glm::vec3
{
    auto const & transformation = part.getGlobalTransformation();

    return transformation[3];
}
