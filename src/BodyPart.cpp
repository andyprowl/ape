#pragma once

#include "BodyPart.hpp"

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

BodyPart::BodyPart(ModelPart const & part)
    : BodyPart{part, nullptr}
{
}

BodyPart::BodyPart(ModelPart const & part, BodyPart & parent)
    : BodyPart{part, &parent}
{
}

auto BodyPart::getComponents() const
    -> std::vector<BodyPart *> const &
{
    return components;
}

auto BodyPart::getModel() const
    -> const ModelPart &
{
    return *part;
}

auto BodyPart::getLocalTransformation() const
    -> glm::mat4 const &
{
    return localTransformation;
}

auto BodyPart::setLocalTransformation(glm::mat4 const & newTransformation)
    -> void
{
    localTransformation = newTransformation;

    updateGlobalTransformation();

    updateGlobalNormalTransformation();

    updateDescendentGlobalTransformations();
}

auto BodyPart::getGlobalTransformation() const
    -> glm::mat4 const &
{
    return globalTransformation;
}

auto BodyPart::getGlobalNormalTransformation() const
    -> glm::mat3 const &
{
    return globalNormalTransformation;
}

// Implemented as a member because it does not require updating the normal matrix
auto BodyPart::scaleUniformly(float const factor)
    -> void
{
    localTransformation = glm::scale(localTransformation, {factor, factor, factor});

    updateGlobalTransformation();
}

// Implemented as a member because it does not require updating the normal matrix
auto BodyPart::translate(glm::vec3 const & offset)
    -> void
{
    localTransformation = glm::translate(localTransformation, offset);

    updateGlobalTransformation();
}

BodyPart::BodyPart(ModelPart const & part, BodyPart * const parent)
    : part{&part}
    , parent{parent}
    , localTransformation{part.getTransformation()}
    , globalTransformation{getParentGlobalTransformation() * localTransformation}
    , globalNormalTransformation{computeNormalTransformation(globalTransformation)}
{
    components.reserve(part.getComponents().size());

    if (parent != nullptr)
    {
        parent->registerComponent(*this);
    }
}

auto BodyPart::getParentGlobalTransformation() const
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

auto BodyPart::registerComponent(BodyPart & component)
    -> void
{
    components.push_back(&component);
}

auto BodyPart::onParentTransformationChanged(glm::mat4 const & newTransformation)
    -> void
{
    globalTransformation = newTransformation * localTransformation;

    updateGlobalNormalTransformation();

    updateDescendentGlobalTransformations();
}

auto BodyPart::updateGlobalTransformation()
    -> void
{
    globalTransformation = getParentGlobalTransformation() * localTransformation;
}

auto BodyPart::updateGlobalNormalTransformation()
    -> void
{
    globalNormalTransformation = computeNormalTransformation(globalTransformation);
}

auto BodyPart::updateDescendentGlobalTransformations() const
    -> void
{
    for (auto component : components)
    {
        component->onParentTransformationChanged(globalTransformation);
    }
}

auto getLocalPosition(BodyPart const & part)
    -> glm::vec3
{
    auto const & transformation = part.getLocalTransformation();

    return transformation[3];
}

auto setLocalPosition(BodyPart & part, glm::vec3 const & newPosition)
    -> void
{
    auto const offset = newPosition - getLocalPosition(part);

    part.translate(offset);
}

auto getGlobalPosition(BodyPart const & part)
    -> glm::vec3
{
    auto const & transformation = part.getGlobalTransformation();

    return transformation[3];
}
