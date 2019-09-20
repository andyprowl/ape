#include "SceneRenderer.hpp"

#include "Material.hpp"
#include "ModelPart.hpp"
#include "Scene.hpp"
#include "ShaderProgram.hpp"
#include "Shape.hpp"

#include "GLFW.hpp"

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
    for (auto i = 0; i < body.getNumOfParts(); ++i)
    {
        auto const & part = body.getPart(i);

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

    for (auto const component : part.getComponents())
    {
        drawBodyPart(*component);
    }
}

auto SceneRenderer::drawMesh(Mesh const & mesh) const
    -> void
{
    auto const & material = mesh.getMaterial();

    uniforms.materialAmbient = material.ambient;

    uniforms.materialShininess = material.shininess;

    material.diffuseMap->bind(0);

    material.specularMap->bind(1);

    mesh.getShape().draw();
}
