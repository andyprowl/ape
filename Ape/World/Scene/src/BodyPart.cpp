#include <Ape/World/Scene/BodyPart.hpp>

#include <Ape/World/Scene/Body.hpp>

#include <Ape/World/Model/ModelPart.hpp>

#include <Basix/Range/Transform.hpp>

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
    , worldTransformation{getParentWorldTransformation() * localTransformation}
    , worldNormalTransformation{computeNormalTransformation(worldTransformation)}
    , meshes{makeMeshes()}
{
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

    auto componentIndex = modelComponent.getInDepthIndex();

    return body->getPart(componentIndex);
}

auto BodyPart::getMeshes() const
    -> std::vector<BodyPartMesh> const &
{
    return meshes;
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

    updateWorldTransformation();

    updateWorldNormalTransformation();

    updateDescendentWorldTransformations();

    body->onLocalTransformationChanged.fire(*this);
}

auto BodyPart::getWorldTransformation() const
    -> glm::mat4 const &
{
    return worldTransformation;
}

auto BodyPart::getWorldNormalTransformation() const
    -> glm::mat3 const &
{
    return worldNormalTransformation;
}

// Implemented as a member because it does not require updating the normal matrix
auto BodyPart::scaleUniformly(float const factor)
    -> void
{
    localTransformation = glm::scale(localTransformation, {factor, factor, factor});

    updateWorldTransformation();

    updateDescendentWorldTransformations();

    body->onLocalTransformationChanged.fire(*this);
}

// Implemented as a member because it does not require updating the normal matrix
auto BodyPart::translate(glm::vec3 const & offset)
    -> void
{
    localTransformation = glm::translate(localTransformation, offset);

    updateWorldTransformation();

    updateDescendentWorldTransformations();

    body->onLocalTransformationChanged.fire(*this);
}

auto BodyPart::setBody(Body & newBody)
    -> void
{
    body = &newBody;
}

auto BodyPart::getParentWorldTransformation() const
    -> glm::mat4
{
    auto const parent = getParent();

    if (parent != nullptr)
    {
        return parent->worldTransformation;
    }
    else
    {
        return glm::mat4{1.0f};
    }
}

auto BodyPart::onParentTransformationChanged(glm::mat4 const & newTransformation)
    -> void
{
    worldTransformation = newTransformation * localTransformation;

    updateWorldNormalTransformation();

    updateDescendentWorldTransformations();

    updateMeshBoundingVolumes();
}

auto BodyPart::updateWorldTransformation()
    -> void
{
    worldTransformation = getParentWorldTransformation() * localTransformation;

    updateMeshBoundingVolumes();
}

auto BodyPart::updateWorldNormalTransformation()
    -> void
{
    worldNormalTransformation = computeNormalTransformation(worldTransformation);
}

auto BodyPart::updateDescendentWorldTransformations()
    -> void
{
    for (auto i = 0; i < getNumOfComponents(); ++i)
    {
        auto & component = getComponent(i);

        component.onParentTransformationChanged(worldTransformation);
    }
}

auto BodyPart::updateMeshBoundingVolumes()
    -> void
{
    for (auto & bounds : meshes)
    {
        bounds.onPartTransformationChanged(worldTransformation);
    }
}

auto BodyPart::makeMeshes() const
    -> std::vector<BodyPartMesh>
{
    return basix::transform(part->getMeshes(), [this] (Mesh const * const mesh)
    {
        return BodyPartMesh{*this, *mesh, worldTransformation};
    });
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
    auto const & transformation = part.getWorldTransformation();

    return transformation[3];
}

} // namespace ape
