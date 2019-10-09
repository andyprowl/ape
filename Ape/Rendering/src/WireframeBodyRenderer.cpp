#include <Rendering/WireframeBodyRenderer.hpp>

#include <Rendering/ShapeRenderer.hpp>
#include <Rendering/WireframeShaderProgram.hpp>

#include <Scene/Body.hpp>
#include <Scene/BodyPart.hpp>
#include <Scene/Camera.hpp>

#include <Model/Mesh.hpp>
#include <Model/ModelPart.hpp>

namespace ape
{

WireframeBodyRenderer::WireframeBodyRenderer(
    WireframeShaderProgram & shader,
    ShapeRenderer const & shapeRenderer,
    LineStyle const & lineStyle)
    : shader{&shader}
    , shapeRenderer{&shapeRenderer}
    , lineStyle{lineStyle}
{
}

auto WireframeBodyRenderer::getLineStyle() const
    -> LineStyle
{
    return lineStyle;
}

auto WireframeBodyRenderer::setLineStyle(LineStyle const & newStyle)
    -> void
{
    lineStyle = newStyle;
}

auto WireframeBodyRenderer::render(BodyRange const & bodies, Camera const & camera) const
    -> void
{
    shader->use();

    shader->lineColor = lineStyle.color;

    shader->lineWidth = lineStyle.width;

    auto const & cameraTransformation = camera.getTransformation();

    for (auto const body : bodies)
    {
        renderBody(*body, cameraTransformation);
    }
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
