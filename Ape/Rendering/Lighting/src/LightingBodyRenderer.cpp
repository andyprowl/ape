#include <Ape/Rendering/Lighting/LightingBodyRenderer.hpp>

#include <Ape/Rendering/Lighting/ShadowMapping.hpp>
#include <Ape/Rendering/Lighting/LightingShaderProgram.hpp>

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

LightingBodyRenderer::LightingBodyRenderer(
    LightingShaderProgram & shader,
    ShapeDrawer const & shapeRenderer)
    : shader{&shader}
    , shapeRenderer{&shapeRenderer}
{
}

auto LightingBodyRenderer::render(
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

auto LightingBodyRenderer::setupInvariantUniforms(
    Camera const & camera,
    LightSystem const & lightSystem,
    ShadowMapping const & shadowMapping) const
    -> void
{
    shader->camera.set(camera);

    shader->lightSystem.set(lightSystem);

    shader->lightSystemView.set(shadowMapping.lightSystemView);

    shader->depthMapping.set(shadowMapping.depthMapping);
}

auto LightingBodyRenderer::renderBody(
    Body const & body,
    glm::mat4 const & cameraTransformation) const
    -> void
{
    for (auto const & part : body.getParts())
    {
        renderBodyPart(part, cameraTransformation);
    }
}

auto LightingBodyRenderer::renderBodyPart(
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

auto LightingBodyRenderer::setupBodyPartUniforms(
    BodyPart const & part,
    glm::mat4 const & cameraTransformation) const
    -> void
{
    auto const & modelTransformation = part.getGlobalTransformation();

    shader->modelTransformation = modelTransformation;
    
    shader->cameraTransformation = cameraTransformation * modelTransformation;

    shader->normalTransformation = part.getGlobalNormalTransformation();
}

auto LightingBodyRenderer::renderMesh(Mesh const & mesh) const
    -> void
{
    auto const & material = mesh.getMaterial();

    shader->material = material;

    auto const & shape = mesh.getShape();
    
    shapeRenderer->render(shape);
}

} // namespace ape
