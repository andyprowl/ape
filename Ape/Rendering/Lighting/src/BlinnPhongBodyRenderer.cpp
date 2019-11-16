#include <Ape/Rendering/Lighting/BlinnPhongBodyRenderer.hpp>

#include <Ape/Rendering/Lighting/ShadowMapping.hpp>
#include <Ape/Rendering/Lighting/BlinnPhongShaderProgram.hpp>

#include <Ape/World/Model/Material.hpp>
#include <Ape/World/Model/Mesh.hpp>
#include <Ape/World/Model/ModelPart.hpp>
#include <Ape/World/Scene/Body.hpp>
#include <Ape/World/Scene/BodyPart.hpp>
#include <Ape/World/Scene/Camera.hpp>
#include <Ape/World/Shape/ShapeDrawer.hpp>

#include <Glow/GpuResource/ScopedBinder.hpp>
#include <Glow/Texture/Texture.hpp>

#include <glad/glad.h>

namespace ape
{

BlinnPhongBodyRenderer::BlinnPhongBodyRenderer(
    BlinnPhongShaderProgram & shader,
    ShapeDrawer const & shapeRenderer)
    : shader{&shader}
    , shapeRenderer{&shapeRenderer}
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

    for (auto const body : bodies)
    {
        renderBody(*body, cameraTransformation);
    }
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
    glm::mat4 const & cameraTransformation) const
    -> void
{
    for (auto const & part : body.getParts())
    {
        renderBodyPart(part, cameraTransformation);
    }
}

auto BlinnPhongBodyRenderer::renderBodyPart(
    BodyPart const & part,
    glm::mat4 const & cameraTransformation) const
    -> void
{
    setupBodyPartUniforms(part, cameraTransformation);

    for (auto const mesh : part.getModel().getMeshes())
    {
        renderMesh(*mesh);
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

auto BlinnPhongBodyRenderer::renderMesh(Mesh const & mesh) const
    -> void
{
    auto const & material = mesh.getMaterial();

    shader->material = material;

    auto const & shape = mesh.getShape();
    
    shapeRenderer->render(shape);
}

} // namespace ape
