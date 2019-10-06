#include <Rendering/StandardBodyRenderer.hpp>

#include <Rendering/ShapeRenderer.hpp>
#include <Rendering/StandardShaderProgram.hpp>

#include <Scene/Body.hpp>
#include <Scene/BodyPart.hpp>

#include <Model/Material.hpp>
#include <Model/Mesh.hpp>
#include <Model/ModelPart.hpp>

namespace ape
{

StandardBodyRenderer::StandardBodyRenderer(
    StandardShaderProgram & shader,
    ShapeRenderer const & shapeRenderer)
    : shader{&shader}
    , shapeRenderer{&shapeRenderer}
{
}

auto StandardBodyRenderer::prepare(Camera const & camera, Lighting const & lighting) const
    -> void
{
    shader->use();

    shader->camera.set(camera);

    shader->lighting.set(lighting);
}

auto StandardBodyRenderer::render(BodyRange const & bodies, Camera const & camera) const
    -> void
{
    shader->use();

    auto const & cameraTransformation = camera.getTransformation();

    for (auto const body : bodies)
    {
        renderBody(*body, cameraTransformation);
    }
}

auto StandardBodyRenderer::renderBody(Body const & body, glm::mat4 const & transformation) const
    -> void
{
    for (auto const & part : body.getParts())
    {
        renderBodyPart(part, transformation);
    }
}

auto StandardBodyRenderer::renderBodyPart(
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
