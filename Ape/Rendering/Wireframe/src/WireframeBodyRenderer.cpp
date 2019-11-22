#include <Ape/Rendering/Wireframe/WireframeBodyRenderer.hpp>

#include <Ape/Rendering/Wireframe/LineStyleProvider.hpp>
#include <Ape/Rendering/Wireframe/WireframeShaderProgram.hpp>

#include <Ape/World/Model/Mesh.hpp>
#include <Ape/World/Model/ModelPart.hpp>
#include <Ape/World/Scene/Body.hpp>
#include <Ape/World/Scene/BodyPart.hpp>
#include <Ape/World/Scene/Camera.hpp>
#include <Ape/World/Shape/ShapeDrawer.hpp>

#include <Glow/GpuResource/ScopedBinder.hpp>

#include <glad/glad.h>

namespace ape
{

WireframeBodyRenderer::WireframeBodyRenderer(
    WireframeShaderProgram & shader,
    ShapeDrawer const & shapeRenderer,
    LineStyleProvider const & styleProvider)
    : shader{&shader}
    , shapeRenderer{&shapeRenderer}
    , styleProvider{&styleProvider}
{
}


auto WireframeBodyRenderer::render(BodyRange const & bodies, Camera const & camera) const
    -> void
{
    auto const shaderBinder = bind(*shader);

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
    auto const & worldTransformation = part.getWorldTransformation();

    shader->transformation = cameraTransformation * worldTransformation;

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
