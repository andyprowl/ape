#include <Ape/Rendering/StandardBodyRenderer.hpp>

#include <Ape/Rendering/ShadowMapping.hpp>
#include <Ape/Rendering/ShapeRenderer.hpp>
#include <Ape/Rendering/StandardShaderProgram.hpp>

#include <Ape/GpuResource/ScopedBinder.hpp>
#include <Ape/GpuResource/Texture.hpp>
#include <Ape/Model/Material.hpp>
#include <Ape/Model/Mesh.hpp>
#include <Ape/Model/ModelPart.hpp>
#include <Ape/Scene/Body.hpp>
#include <Ape/Scene/BodyPart.hpp>
#include <Ape/Scene/Camera.hpp>

#include <glad/glad.h>

namespace ape
{

StandardBodyRenderer::StandardBodyRenderer(
    StandardShaderProgram & shader,
    ShapeRenderer const & shapeRenderer)
    : shader{&shader}
    , shapeRenderer{&shapeRenderer}
    , viewport{{0, 0}, {0, 0}}
{
}

auto StandardBodyRenderer::render(
    BodyRange const & bodies,
    Camera const & camera,
    Lighting const & lighting,
    ShadowMapping const & shadowMapping) const
    -> void
{
    auto const shaderBinder = ScopedBinder{*shader};

    setupViewport();

    setupInvariantUniforms(camera, lighting, shadowMapping);

    auto const & cameraTransformation = camera.getTransformation();

    for (auto const body : bodies)
    {
        renderBody(*body, cameraTransformation);
    }
}

auto StandardBodyRenderer::setViewport(Viewport const & newViewport)
    -> void
{
    viewport = newViewport;
}

auto StandardBodyRenderer::setupViewport() const
    -> void
{
    glViewport(viewport.origin.x, viewport.origin.y, viewport.size.width, viewport.size.height);
}

auto StandardBodyRenderer::setupInvariantUniforms(
    Camera const & camera,
    Lighting const & lighting,
    ShadowMapping const & shadowMapping) const
    -> void
{
    shader->camera.set(camera);

    shader->lighting.set(lighting);

    shader->lightingView.set(shadowMapping.lightingView);

    shader->depthMapping.set(shadowMapping.depthMapping);
}

auto StandardBodyRenderer::renderBody(
    Body const & body,
    glm::mat4 const & cameraTransformation) const
    -> void
{
    for (auto const & part : body.getParts())
    {
        renderBodyPart(part, cameraTransformation);
    }
}

auto StandardBodyRenderer::renderBodyPart(
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

auto StandardBodyRenderer::setupBodyPartUniforms(
    BodyPart const & part,
    glm::mat4 const & cameraTransformation) const
    -> void
{
    auto const & modelTransformation = part.getGlobalTransformation();

    shader->modelTransformation = modelTransformation;
    
    shader->cameraTransformation = cameraTransformation * modelTransformation;

    shader->normalTransformation = part.getGlobalNormalTransformation();
}

auto StandardBodyRenderer::renderMesh(Mesh const & mesh) const
    -> void
{
    auto const & material = mesh.getMaterial();

    shader->material = material;

    auto const & shape = mesh.getShape();
    
    shapeRenderer->render(shape);
}

} // namespace ape
