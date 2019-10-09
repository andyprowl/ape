#include <Rendering/StandardBodyRenderer.hpp>

#include <Rendering/ShapeRenderer.hpp>
#include <Rendering/StandardShaderProgram.hpp>

#include <Scene/Body.hpp>
#include <Scene/BodyPart.hpp>
#include <Scene/Camera.hpp>

#include <Model/Material.hpp>
#include <Model/Mesh.hpp>
#include <Model/ModelPart.hpp>

#include <glad/glad.h>

namespace ape
{

StandardBodyRenderer::StandardBodyRenderer(
    StandardShaderProgram & shader,
    DepthShaderProgram & depthShader,
    ShapeRenderer const & shapeRenderer)
    : shader{&shader}
    , depthMapRenderer{depthShader, shapeRenderer}
    , shapeRenderer{&shapeRenderer}
    , viewport{{0, 0}, {0, 0}}
{
}

auto StandardBodyRenderer::render(
    BodyRange const & bodies,
    Camera const & camera,
    Lighting const & lighting) const
    -> void
{
    auto const & light = lighting.spot.front();

    auto const depthMapSize = depthMapRenderer.getDepthMapSize();

    auto const aspectRatio = depthMapSize.width / static_cast<float>(depthMapSize.height);

    auto const lightView = Camera{
        light.position,
        light.direction,
        {0.0f, 1.0f, 0.0f},
        light.cutoff.outer * 2.0f,
        aspectRatio};

    depthMapRenderer.render(bodies, lightView);

    renderRegularScene(bodies, camera, lighting, lightView);
}

auto StandardBodyRenderer::setViewport(Viewport const & newViewport)
    -> void
{
    viewport = newViewport;
}

auto StandardBodyRenderer::renderDepthMap(BodyRange const & bodies, Camera const & lightView) const
    -> void
{
    depthMapRenderer.render(bodies, lightView);
}

auto StandardBodyRenderer::renderRegularScene(
    BodyRange const & bodies,
    Camera const & camera,
    Lighting const & lighting,
    Camera const & lightView) const
    -> void
{
    glViewport(viewport.origin.x, viewport.origin.y, viewport.size.width, viewport.size.height);

    shader->use();

    shader->camera.set(camera);

    shader->lighting.set(lighting);

    depthMapRenderer.getDepthMap().bind(2);

    auto const & cameraTransformation = camera.getTransformation();

    auto const & lightTransformation = lightView.getTransformation();

    for (auto const body : bodies)
    {
        renderBody(*body, cameraTransformation, lightTransformation);
    }
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
