#include <Scene/BodyPart.hpp>

#include <Scene/Body.hpp>

#include <Asset/ModelPart.hpp>

#include <glm/gtc/matrix_transform.hpp>

namespace ape
{

namespace
{

auto computeNormalTransformation(glm::mat4 const & transformation)
    -> glm::mat3
{
    return glm::mat3{glm::transpose(glm::inverse(transformation))};
}

} // unnamed namespace

BodyPart::BodyPart(Body & body, ModelPart const & part)
    : body{&body}
    , part{&part}
    , localTransformation{part.getTransformation()}
    , globalTransformation{getParentGlobalTransformation() * localTransformation}
    , globalNormalTransformation{computeNormalTransformation(globalTransformation)}
{
}

auto BodyPart::getNumOfComponents() const
    -> int
{
    return static_cast<int>(part->getComponents().size());
}

auto BodyPart::getComponent(int const index)
    -> BodyPart &
{
    auto & modelComponents = part->getComponents();

    auto & modelComponent = modelComponents[index];

    auto inDepthIndex = modelComponent.getInDepthIndex();

    return body->getPart(inDepthIndex);
}

auto BodyPart::getBody() const
    -> Body const &
{
    return *body;
}

auto BodyPart::getModel() const
    -> const ModelPart &
{
    return *part;
}

auto BodyPart::getParent() const
    -> const BodyPart *
{
    auto const modelParent = part->getParent();

    if (modelParent == nullptr)
    {
        return nullptr;
    }

    auto const parentIndex = modelParent->getInDepthIndex();

    auto const & parentPart = body->getPart(parentIndex);

    return &parentPart;
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

    body->onLocalTransformationChanged.fire(*this);
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

    body->onLocalTransformationChanged.fire(*this);
}

// Implemented as a member because it does not require updating the normal matrix
auto BodyPart::translate(glm::vec3 const & offset)
    -> void
{
    localTransformation = glm::translate(localTransformation, offset);

    updateGlobalTransformation();

    body->onLocalTransformationChanged.fire(*this);
}

auto BodyPart::setBody(Body & newBody)
    -> void
{
    body = &newBody;
}

auto BodyPart::getParentGlobalTransformation() const
    -> glm::mat4
{
    auto const parent = getParent();

    if (parent != nullptr)
    {
        return parent->globalTransformation;
    }
    else
    {
        return glm::mat4{1.0f};
    }
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

auto BodyPart::updateDescendentGlobalTransformations()
    -> void
{
    for (auto i = 0; i < getNumOfComponents(); ++i)
    {
        auto & component = getComponent(i);

        component.onParentTransformationChanged(globalTransformation);
    }
}

auto isRoot(BodyPart const & part)
    -> bool
{
    return isRoot(part.getModel());
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

} // namespace ape
