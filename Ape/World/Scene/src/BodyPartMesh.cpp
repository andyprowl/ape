#include <Ape/World/Scene/BodyPartMesh.hpp>

#include <Ape/World/Model/Mesh.hpp>
#include <Ape/World/Shape/Shape.hpp>

namespace ape
{

namespace
{

auto makeBoundingVolumesInWorldCoordinates(
    Mesh const & model,
    glm::mat4 const & worldTransformation)
    -> ShapeBounds
{
    auto modelBounds = model.getShape().getBoundingVolumes();

    modelBounds.transform(worldTransformation);

    return modelBounds;
}

} // unnamed namespace

BodyPartMesh::BodyPartMesh(
    Mesh const & model,
    glm::mat4 const & worldTransformation)
    : model{&model}
    , boundingVolumes{makeBoundingVolumesInWorldCoordinates(model, worldTransformation)}
{
}
    
auto BodyPartMesh::getModel() const
    -> Mesh const &
{
    return *model;
}

auto BodyPartMesh::getBoundingVolumes() const
    -> const ShapeBounds &
{
    return boundingVolumes;
}

auto BodyPartMesh::onPartTransformationChanged(glm::mat4 const & worldTransformation)
    -> void
{
    boundingVolumes = model->getShape().getBoundingVolumes();

    boundingVolumes.transform(worldTransformation);
}

} // namespace ape
