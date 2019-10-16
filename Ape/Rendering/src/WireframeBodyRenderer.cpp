#include <Rendering/WireframeBodyRenderer.hpp>

#include <Rendering/LineStyleProvider.hpp>
#include <Rendering/ShapeRenderer.hpp>
#include <Rendering/WireframeShaderProgram.hpp>

#include <Model/Mesh.hpp>
#include <Model/ModelPart.hpp>

#include <Scene/Body.hpp>
#include <Scene/BodyPart.hpp>
#include <Scene/Camera.hpp>

#include <GpuResource/ScopedBinder.hpp>

namespace ape
{

WireframeBodyRenderer::WireframeBodyRenderer(
    WireframeShaderProgram & shader,
    ShapeRenderer const & shapeRenderer,
    LineStyleProvider const & styleProvider)
    : shader{&shader}
    , shapeRenderer{&shapeRenderer}
    , styleProvider{&styleProvider}
{
}

auto WireframeBodyRenderer::render(BodyRange const & bodies, Camera const & camera) const
    -> void
{
    auto const shaderBinder = ScopedBinder{*shader};

    setupLineStyleUniforms();

    auto const & cameraTransformation = camera.getTransformation();

    for (auto const body : bodies)
    {
        renderBody(*body, cameraTransformation);
    }
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
