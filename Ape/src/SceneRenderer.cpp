#include <Ape/SceneRenderer.hpp>

#include <Ape/Material.hpp>
#include <Ape/ModelPart.hpp>
#include <Ape/Scene.hpp>
#include <Ape/ShaderProgram.hpp>
#include <Ape/Shape.hpp>

#include <glad/glad.h>

SceneRenderer::SceneRenderer(ShaderProgram const & shader, glm::vec3 const & backgroundColor)
    : shader{&shader}
    , backgroundColor{backgroundColor}
    , uniforms{shader}
{
    glEnable(GL_DEPTH_TEST);
}

auto SceneRenderer::render(Scene const & s) const
    -> void
{
    clear();

    shader->use();

    uniforms.camera.set(*s.cameraSystem.activeCamera);

    uniforms.lighting.set(s.lighting);

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
    uniforms.modelTransformation = part.getGlobalTransformation();

    uniforms.normalMatrix = part.getGlobalNormalTransformation();

    for (auto const mesh : part.getModel().getMeshes())
    {
        drawMesh(*mesh);
    }
}

auto SceneRenderer::drawMesh(Mesh const & mesh) const
    -> void
{
    auto const & material = mesh.getMaterial();

    uniforms.materialAmbient = material.ambient;

    uniforms.materialShininess = material.shininess;

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