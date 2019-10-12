#include <Rendering/StandardBodyRenderer.hpp>

#include <Rendering/ShadowMapping.hpp>
#include <Rendering/ShapeRenderer.hpp>
#include <Rendering/StandardShaderProgram.hpp>

#include <Scene/Body.hpp>
#include <Scene/BodyPart.hpp>
#include <Scene/Camera.hpp>

#include <Model/Material.hpp>
#include <Model/Mesh.hpp>
#include <Model/ModelPart.hpp>

#include <GpuResource/Texture.hpp>

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
    glViewport(viewport.origin.x, viewport.origin.y, viewport.size.width, viewport.size.height);

    shader->use();

    shader->camera.set(camera);

    shader->lighting.set(lighting);

    // TODO: Hardcoding #2 is not correct. Texture unit should be determined in some other way.
    shadowMapping.depthMapping.getSpotMapping()[0].getTexture().bind(2);

    auto const & lightView = shadowMapping.lightingView.getSpotView()[0];

    auto const & cameraTransformation = camera.getTransformation();

    auto const & lightTransformation = lightView.getTransformation();

    for (auto const body : bodies)
    {
        renderBody(*body, cameraTransformation, lightTransformation);
    }
}

auto StandardBodyRenderer::setViewport(Viewport const & newViewport)
    -> void
{
    viewport = newViewport;
}

auto StandardBodyRenderer::renderBody(
    Body const & body,
    glm::mat4 const & cameraTransformation,
    glm::mat4 const & lightTransformation) const
    -> void
{
    for (auto const & part : body.getParts())
    {
        renderBodyPart(part, cameraTransformation, lightTransformation);
    }
}

auto StandardBodyRenderer::renderBodyPart(
    BodyPart const & part,
    glm::mat4 const & cameraTransformation,
    glm::mat4 const & lightTransformation) const
    -> void
{
    auto const & modelTransformation = part.getGlobalTransformation();

    shader->modelTransformation = modelTransformation;
    
    shader->cameraTransformation = cameraTransformation * modelTransformation;

    shader->normalTransformation = part.getGlobalNormalTransformation();

    shader->lightTransformation = lightTransformation * modelTransformation;

    for (auto const mesh : part.getModel().getMeshes())
    {
        renderMesh(*mesh);
    }
}

auto StandardBodyRenderer::renderMesh(Mesh const & mesh) const
    -> void
{
    auto const & material = mesh.getMaterial();

    shader->materialAmbient = material.ambient;

    shader->materialShininess = material.shininess;

    if (material.diffuseMap)
    {
        material.diffuseMap->bind(0);
    }
    
    if (material.specularMap)
    {
        material.specularMap->bind(1);
    }

    auto const & shape = mesh.getShape();
    
    shapeRenderer->render(shape);
}

} // namespace ape
