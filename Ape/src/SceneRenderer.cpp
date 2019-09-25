#include <Ape/SceneRenderer.hpp>

#include <Ape/Camera.hpp>
#include <Ape/CameraSelector.hpp>
#include <Ape/Material.hpp>
#include <Ape/ModelPart.hpp>
#include <Ape/Scene.hpp>
#include <Ape/ShaderProgram.hpp>
#include <Ape/Shape.hpp>
#include <Ape/StandardShaderProgram.hpp>

#include <glad/glad.h>

#include <algorithm>
#include <cassert>

namespace ape
{

SceneRenderer::SceneRenderer(
    CameraSelector const & cameraSelector,
    StandardShaderProgram & shader,
    glm::vec3 const & backgroundColor)
    : cameraSelector{&cameraSelector}
    , shader{&shader}
    , backgroundColor{backgroundColor}
    , vaoId{0}
{
    glGenVertexArrays(1, &vaoId);
}

auto SceneRenderer::render() const
    -> void
{
    clear();

    auto const camera = cameraSelector->getActiveCamera();

    if (camera == nullptr)
    {
        return;
    }

    shader->use();

    shader->camera.set(*camera);

    auto const & scene = cameraSelector->getScene();

    shader->lighting.set(scene.lighting);

    drawScene(scene, *camera);
}

auto SceneRenderer::getCameraSelector() const
    -> CameraSelector const &
{
    return *cameraSelector;
}

auto SceneRenderer::setCameraSelector(CameraSelector const & newSelector)
    -> void
{
    cameraSelector = &newSelector;
}

auto SceneRenderer::clear() const
    -> void
{
    glEnable(GL_DEPTH_TEST);

    glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

auto SceneRenderer::drawScene(Scene const & scene, Camera const & camera) const
    -> void
{
    glBindVertexArray(vaoId);

    auto const & cameraTransformation = camera.getTransformation();

    for (auto const & body : scene.bodies)
    {
        drawBody(body, cameraTransformation);
    }

    glBindVertexArray(0);
}

auto SceneRenderer::drawBody(Body const & body, glm::mat4 const & cameraTransformation) const
    -> void
{
    for (auto const & part : body.getParts())
    {
        drawBodyPart(part, cameraTransformation);
    }
}

auto SceneRenderer::drawBodyPart(
    BodyPart const & part,
    glm::mat4 const & cameraTransformation) const
    -> void
{
    auto const & modelTransformation = part.getGlobalTransformation();

    shader->modelTransformation = modelTransformation;

    shader->cameraTransformation = cameraTransformation * modelTransformation;

    shader->normalTransformation = part.getGlobalNormalTransformation();

    for (auto const mesh : part.getModel().getMeshes())
    {
        drawMesh(*mesh);
    }
}

auto SceneRenderer::drawMesh(Mesh const & mesh) const
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

    mesh.getShape().draw();
}

auto getCamera(SceneRenderer const & renderer)
    -> Camera *
{
    return renderer.getCameraSelector().getActiveCamera();
}

auto getScene(SceneRenderer const & renderer)
    -> Scene &
{
    return renderer.getCameraSelector().getScene();
}

} // namespace ape
