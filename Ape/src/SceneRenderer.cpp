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

SceneRenderer::SceneRenderer(StandardShaderProgram & shader, glm::vec3 const & backgroundColor)
    : shader{&shader}
    , backgroundColor{backgroundColor}
{
    glEnable(GL_DEPTH_TEST);
}

auto SceneRenderer::render(Scene const & s) const
    -> void
{
    clear();

    shader->use();

    shader->camera.set(*s.cameraSystem.activeCamera);

    shader->lighting.set(s.lighting);

    drawScene(s);
}

auto SceneRenderer::clear() const
    -> void
{
    glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

auto SceneRenderer::drawScene(Scene const & scene) const
    -> void
{
    for (auto const & body : scene.bodies)
    {
        drawBody(body);
    }
}

auto SceneRenderer::drawBody(Body const & body) const
    -> void
{
    for (auto const & part : body.getParts())
    {
        drawBodyPart(part);
    }
}

auto SceneRenderer::drawBodyPart(BodyPart const & part) const
    -> void
{
    shader->modelTransformation = part.getGlobalTransformation();

    shader->normalMatrix = part.getGlobalNormalTransformation();

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
