#include <Ape/SceneRenderer.hpp>

#include <Ape/Material.hpp>
#include <Ape/ModelPart.hpp>
#include <Ape/Scene.hpp>
#include <Ape/ShaderProgram.hpp>
#include <Ape/Shape.hpp>
#include <Ape/StandardShaderProgram.hpp>

#include <glad/glad.h>

namespace ape
{

SceneRenderer::SceneRenderer(
    Scene const & scene,
    StandardShaderProgram & shader,
    glm::vec3 const & backgroundColor)
    : scene{&scene}
    , shader{&shader}
    , backgroundColor{backgroundColor}
{
    glEnable(GL_DEPTH_TEST);
}

auto SceneRenderer::render() const
    -> void
{
    clear();

    shader->use();

    shader->camera.set(*(scene->cameraSystem.activeCamera));

    shader->lighting.set(scene->lighting);

    drawScene();
}

auto SceneRenderer::getScene() const
    -> Scene const &
{
    return *scene;
}

auto SceneRenderer::clear() const
    -> void
{
    glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

auto SceneRenderer::drawScene() const
    -> void
{
    auto const & cameraTransformation = scene->cameraSystem.activeCamera->getTransformation();

    for (auto const & body : scene->bodies)
    {
        drawBody(body, cameraTransformation);
    }
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

} // namespace ape
