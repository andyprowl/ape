#include <Ape/Rendering/Lighting/BlinnPhongBodyRenderer.hpp>

#include <Ape/Rendering/Lighting/ShadowMapping.hpp>
#include <Ape/Rendering/Lighting/BlinnPhongShaderProgram.hpp>

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
#include <Glow/Texture/Texture.hpp>

#include <Basix/Range/Sorting.hpp>

#include <glad/glad.h>

namespace ape
{

namespace
{

auto getDistance(Body const & body, Camera const & camera)
    -> float
{
    auto const & eye = camera.getView().getPosition();

    for (auto const & part : body.getParts())
    {
        auto const & meshes = part.getMeshes();

        if (!meshes.empty())
        {
            auto const& boundingSphere = meshes.front().getBoundingVolumes().getSphere();

            return (glm::distance(boundingSphere.getCenter(), eye) - boundingSphere.getRadius());
        }
    }

    return 0.0;
}

} // unnamed namespace

BlinnPhongBodyRenderer::BlinnPhongBodyRenderer(
    BlinnPhongShaderProgram & shader,
    ShapeDrawer const & shapeRenderer)
    : shader{&shader}
    , shapeRenderer{&shapeRenderer}
    , performFrustumCulling{true}
{
}

auto BlinnPhongBodyRenderer::render(
    BodyRange const & bodies,
    Camera const & camera,
    LightSystem const & lightSystem,
    ShadowMapping const & shadowMapping) const
    -> void
{
    auto const shaderBinder = bind(*shader);

    setupInvariantUniforms(camera, lightSystem, shadowMapping);

    auto const & cameraTransformation = camera.getTransformation();

    auto const culler = RadarFrustumCuller{camera};

    auto sortedBodies = getBodiesSortedByDistanceFromCamera(bodies, camera);

    for (auto const body : sortedBodies)
    {
        renderBody(*body, cameraTransformation, culler);
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

auto BlinnPhongBodyRenderer::getBodiesSortedByDistanceFromCamera(
    BodyRange const & bodies,
    Camera const & camera) const
    -> std::vector<Body const*>
{
    auto sortedBodies = std::vector<Body const *>{std::cbegin(bodies), std::cend(bodies)};

    basix::sort(sortedBodies, [&camera] (Body const * const lhs, Body const * const rhs)
    {
        return (getDistance(*lhs, camera) < getDistance(*rhs, camera));
    });

    return sortedBodies;
}

auto BlinnPhongBodyRenderer::setupInvariantUniforms(
    Camera const & camera,
    LightSystem const & lightSystem,
    ShadowMapping const & shadowMapping) const
    -> void
{
    shader->cameraPosition.set(camera.getView().getPosition());

    shader->lightSystem.set(lightSystem);

    shader->lightSystemView.set(shadowMapping.lightSystemView);

    shader->depthMapping.set(shadowMapping.depthMapping);
}

auto BlinnPhongBodyRenderer::renderBody(
    Body const & body,
    glm::mat4 const & cameraTransformation,
    RadarFrustumCuller const & culler) const
    -> void
{
    for (auto const & part : body.getParts())
    {
        renderBodyPart(part, cameraTransformation, culler);
    }
}

auto BlinnPhongBodyRenderer::renderBodyPart(
    BodyPart const & part,
    glm::mat4 const & cameraTransformation,
    RadarFrustumCuller const & culler) const
    -> void
{
    setupBodyPartUniforms(part, cameraTransformation);

    for (auto const & bodyMesh : part.getMeshes())
    {
        if (!isVisible(bodyMesh, culler))
        {
            continue;
        }

        renderMesh(bodyMesh);
    }
}

auto BlinnPhongBodyRenderer::setupBodyPartUniforms(
    BodyPart const & part,
    glm::mat4 const & cameraTransformation) const
    -> void
{
    auto const & worldTransformation = part.getWorldTransformation();

    shader->worldTransformation = worldTransformation;
    
    shader->cameraTransformation = cameraTransformation * worldTransformation;

    shader->normalTransformation = part.getWorldNormalTransformation();
}

auto BlinnPhongBodyRenderer::isVisible(
    BodyPartMesh const & mesh,
    RadarFrustumCuller const & culler) const
    -> bool
{
    if (!isFrustumCullingEnabled())
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

    shader->material = material;

    auto const & shape = meshModel.getShape();
    
    shapeRenderer->render(shape);
}

} // namespace ape
