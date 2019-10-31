#include <Ape/Rendering/WireframeBodyRenderer.hpp>

#include <Ape/Rendering/LineStyleProvider.hpp>
#include <Ape/Rendering/ShapeRenderer.hpp>
#include <Ape/Rendering/WireframeShaderProgram.hpp>

#include <Ape/GpuResource/ScopedBinder.hpp>
#include <Ape/Model/Mesh.hpp>
#include <Ape/Model/ModelPart.hpp>
#include <Ape/Scene/Body.hpp>
#include <Ape/Scene/BodyPart.hpp>
#include <Ape/Scene/Camera.hpp>

#include <glad/glad.h>

namespace ape
{

WireframeBodyRenderer::WireframeBodyRenderer(
    WireframeShaderProgram & shader,
    ShapeRenderer const & shapeRenderer,
    LineStyleProvider const & styleProvider)
    : shader{&shader}
    , shapeRenderer{&shapeRenderer}
    , styleProvider{&styleProvider}
    , viewport{{0, 0}, {0, 0}}
{
}

auto WireframeBodyRenderer::setViewport(Viewport const & newViewport)
    -> void
{
    viewport = newViewport;
}

auto WireframeBodyRenderer::render(BodyRange const & bodies, Camera const & camera) const
    -> void
{
    auto const shaderBinder = bind(*shader);

    setupViewport();

    setupLineStyleUniforms();

    auto const & cameraTransformation = camera.getTransformation();

    for (auto const body : bodies)
    {
        renderBody(*body, cameraTransformation);
    }
}

auto WireframeBodyRenderer::setupViewport() const
    -> void
{
    glViewport(viewport.origin.x, viewport.origin.y, viewport.size.width, viewport.size.height);
}

auto WireframeBodyRenderer::setupLineStyleUniforms() const
    -> void
{
    auto const & lineStyle = styleProvider->getStyle();

    shader->lineColor = lineStyle.color;

    shader->lineWidth = lineStyle.width;
}

auto WireframeBodyRenderer::renderBody(
    Body const & body,
    glm::mat4 const & transformation) const
    -> void
{
    for (auto const & part : body.getParts())
    {
        renderBodyPart(part, transformation);
    }
}

auto WireframeBodyRenderer::renderBodyPart(
    BodyPart const & part,
    glm::mat4 const & cameraTransformation) const
    -> void
{
    auto const & modelTransformation = part.getGlobalTransformation();

    shader->transformation = cameraTransformation * modelTransformation;

    for (auto const mesh : part.getModel().getMeshes())
    {
        renderMesh(*mesh);
    }
}

auto WireframeBodyRenderer::renderMesh(Mesh const & mesh) const
    -> void
{
    auto const & shape = mesh.getShape();
    
    shapeRenderer->render(shape);
}

} // namespace ape
