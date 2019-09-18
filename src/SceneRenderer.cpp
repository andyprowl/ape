#include "SceneRenderer.hpp"

#include "Scene.hpp"
#include "ShaderProgram.hpp"

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

    uniforms.camera.set(s.camera);

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

auto SceneRenderer::drawScene(Scene const & s) const
    -> void
{
    for (auto const & mesh : s.meshes)
    {
        drawMesh(mesh);
    }
}

auto SceneRenderer::drawMesh(Mesh const & m) const
    -> void
{
    auto const material = m.getMaterial();

    uniforms.modelTransformation = m.getModelTransformation();

    uniforms.normalMatrix = m.getNormalMatrix();

    uniforms.materialAmbient = material.ambient;

    uniforms.materialShininess = material.shininess;

    material.diffuseMap.bind(0);

    material.specularMap.bind(1);

    m.getShape().draw();
}
