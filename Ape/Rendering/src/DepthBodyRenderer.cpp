#include <Rendering/DepthBodyRenderer.hpp>

#include <Rendering/DepthShaderProgram.hpp>
#include <Rendering/ShapeRenderer.hpp>

#include <Scene/Body.hpp>
#include <Scene/BodyPart.hpp>
#include <Scene/Camera.hpp>

#include <GpuResource/ScopedBinder.hpp>

#include <Model/Mesh.hpp>
#include <Model/ModelPart.hpp>

#include <glad/glad.h>

namespace ape
{

namespace
{

auto const depthMapSize = Size<int>{1'024, 1'024};

auto asReference(Body const & body)
    -> Body const &
{
    return body;
}

auto asReference(Body * const body)
    -> Body const &
{
    return *body;
}

} // unnamed namespace

DepthBodyRenderer::DepthBodyRenderer(
    DepthShaderProgram & shader,
    ShapeRenderer const & shapeRenderer)
    : shader{&shader}
    , shapeRenderer{&shapeRenderer}
    , depthMap{depthMapSize}
{
}

auto DepthBodyRenderer::render(BodyRange const & bodies, Camera const & lightView) const
    -> void
{
    renderBodies(bodies, lightView);
}

auto DepthBodyRenderer::render(BodyContainerView const & bodies, Camera const & lightView) const
    -> void
{
    renderBodies(bodies, lightView);
}

auto DepthBodyRenderer::getDepthMap() const
    -> DepthMap const &
{
    return depthMap;
}

auto DepthBodyRenderer::getDepthMapSize() const
    -> Size<int>
{
    return depthMap.getSize();
}

template<typename Range>
auto DepthBodyRenderer::renderBodies(Range const & bodies, Camera const & lightView) const
    -> void
{
    shader->use();

    auto const binder = ScopedBinder{depthMap.getFrameBuffer()};

    auto const mapSize = depthMap.getSize();

    glViewport(0, 0, mapSize.width, mapSize.height);

    glClear(GL_DEPTH_BUFFER_BIT);

    auto const & lightTransformation = lightView.getTransformation();

    for (auto const & body : bodies)
    {
        renderBody(asReference(body), lightTransformation);
    }
}

auto DepthBodyRenderer::renderBody(
    Body const & body,
    glm::mat4 const & lightTransformation) const
    -> void
{
    for (auto const & part : body.getParts())
    {
        renderBodyPart(part, lightTransformation);
    }
}

auto DepthBodyRenderer::renderBodyPart(
    BodyPart const & part,
    glm::mat4 const & lightTransformation) const
    -> void
{
    auto const & modelTransformation = part.getGlobalTransformation();
    
    shader->lightTransformation = lightTransformation * modelTransformation;

    for (auto const mesh : part.getModel().getMeshes())
    {
        renderMesh(*mesh);
    }
}

auto DepthBodyRenderer::renderMesh(Mesh const & mesh) const
    -> void
{
    auto const & shape = mesh.getShape();
    
    shapeRenderer->render(shape);
}

} // namespace ape
