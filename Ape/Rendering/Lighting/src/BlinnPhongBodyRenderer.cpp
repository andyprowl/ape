#include <Ape/Rendering/Lighting/BlinnPhongBodyRenderer.hpp>

#include <Ape/Rendering/Lighting/BlinnPhongShaderProgram.hpp>
#include <Ape/Rendering/Lighting/LightSystemUniformSetter.hpp>
#include <Ape/Rendering/Lighting/LightSystemViewUniformSetter.hpp>
#include <Ape/Rendering/Lighting/MaterialSetUniformSetter.hpp>
#include <Ape/Rendering/Lighting/ShadowMapping.hpp>

#include <Ape/Rendering/Culling/RadarFrustumCuller.hpp>

#include <Ape/World/Model/Material.hpp>
#include <Ape/World/Model/Mesh.hpp>
#include <Ape/World/Model/Model.hpp>
#include <Ape/World/Model/ModelPart.hpp>
#include <Ape/World/Scene/Body.hpp>
#include <Ape/World/Scene/BodyPart.hpp>
#include <Ape/World/Scene/Camera.hpp>
#include <Ape/World/Shape/ShapeDrawer.hpp>

#include <Glow/GpuResource/ScopedBinder.hpp>
#include <Glow/Texture/Texture2d.hpp>

#include <Basix/Range/Sorting.hpp>

#include <glad/glad.h>

namespace ape
{

namespace
{

auto getDistance(BodyPartMesh const & mesh, Camera const & camera)
    -> float
{
    auto const & eye = camera.getView().getPosition();

    auto const& boundingSphere = mesh.getBoundingVolumes().getSphere();

    return (glm::distance(boundingSphere.getCenter(), eye) - boundingSphere.getRadius());
}

} // unnamed namespace

BlinnPhongBodyRenderer::BlinnPhongBodyRenderer(
    BlinnPhongShaderProgram & shader,
    LightSystemUniformSetter & lightSystemSetter,
    LightSystemViewUniformSetter & lightSystemViewSetter,
    MaterialSetUniformSetter & materialSetSetter,
    ShapeDrawer & shapeRenderer)
    : shader{&shader}
    , lightSystemSetter{&lightSystemSetter}
    , lightSystemViewSetter{&lightSystemViewSetter}
    , materialSetSetter{&materialSetSetter}
    , shapeRenderer{&shapeRenderer}
    , performFrustumCulling{true}
{
}

auto BlinnPhongBodyRenderer::render(
    BodyRange const & bodies,
    Camera const & camera,
    Fog const & fog,
    ShadowMapping const & shadowMapping) const
    -> void
{
    auto const shaderBinder = glow::bind(*shader);

    setupInvariantUniforms(camera, fog, shadowMapping);

    auto const sortedMeshes = getVisibleMeshesSortedByDistanceFromCamera(bodies, camera);

    for (auto const mesh : sortedMeshes)
    {
        auto const & part = mesh->getPart();

        setupBodyPartUniforms(part);
    
        renderMesh(*mesh);
    }
}

auto BlinnPhongBodyRenderer::isFrustumCullingEnabled() const
    -> bool
{
    return performFrustumCulling;
}

auto BlinnPhongBodyRenderer::enableFrustumCulling(bool const enable)
    -> void
{
    performFrustumCulling = enable;
}

auto BlinnPhongBodyRenderer::setupInvariantUniforms(
    Camera const & camera,
    Fog const & fog,
    ShadowMapping const & shadowMapping) const
    -> void
{
    lightSystemSetter->flush();

    glow::setBlockDataSource(lightSystemSetter->getUniformBuffer(), shader->lightSystem);

    lightSystemViewSetter->flush();

    glow::setBlockDataSource(lightSystemViewSetter->getUniformBuffer(), shader->lightSystemView);

    materialSetSetter->flush();

    glow::setBlockDataSource(materialSetSetter->getUniformBuffer(), shader->materialSet);

    shader->cameraPosition.set(camera.getView().getPosition());
    
    shader->worldToClipTransformation.set(camera.getTransformation());

    shader->depthMapping.set(shadowMapping.depthMapping);

    shader->fog.set(fog);
}

auto BlinnPhongBodyRenderer::getVisibleMeshesSortedByDistanceFromCamera(
    BodyRange const & bodies,
    Camera const & camera) const
    -> std::vector<const BodyPartMesh *>
{
    auto meshes = getVisibleMeshes(bodies, camera);

    basix::sort(
        meshes,
        [&camera] (BodyPartMesh const * const lhs, BodyPartMesh const * const rhs)
    {
        return (getDistance(*lhs, camera) < getDistance(*rhs, camera));
    });

    return meshes;
}

auto BlinnPhongBodyRenderer::getVisibleMeshes(
    BodyRange const & bodies,
    Camera const & camera) const
    -> std::vector<const BodyPartMesh *>
{
    auto const culler = RadarFrustumCuller{camera};

    auto meshes = std::vector<BodyPartMesh const *>{};

    for (auto const body : bodies)
    {
        for (auto const & part : body->getParts())
        {
            for (auto const & mesh : part.getMeshes())
            {
                if (isVisible(mesh, culler))
                {
                    meshes.push_back(&mesh);
                }
            }
        }
    }

    return meshes;
}

auto BlinnPhongBodyRenderer::setupBodyPartUniforms(BodyPart const & part) const
    -> void
{
    auto const & worldTransformation = part.getWorldTransformation();

    shader->objectToWorldTransformation.set(worldTransformation);

    auto const & normalTransformation = part.getWorldNormalTransformation();

    shader->normalTransformation.set(normalTransformation);
}

auto BlinnPhongBodyRenderer::isVisible(BodyPartMesh const & mesh, Culler const & culler) const
    -> bool
{
    if (not isFrustumCullingEnabled())
    {
        return true;
    }

    auto const & boundingSphere = mesh.getBoundingVolumes().getSphere();

    auto const relation = culler.isSphereContained(boundingSphere);

    return (relation != ContainmentRelation::fullyOutside);
}

auto BlinnPhongBodyRenderer::renderMesh(BodyPartMesh const & mesh) const
    -> void
{
    auto const & meshModel = mesh.getModel();

    auto const & material = meshModel.getMaterial();

    shader->materialMaps.set(material);

    shader->activeMaterialIndex.set(material.getInstanceIndex());

    auto const & shape = meshModel.getShape();
    
    shapeRenderer->render(shape);
}

} // namespace ape
