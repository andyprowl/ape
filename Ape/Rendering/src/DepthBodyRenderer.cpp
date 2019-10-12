#include <Rendering/DepthBodyRenderer.hpp>

#include <Rendering/DepthMapping.hpp>
#include <Rendering/DepthShaderProgram.hpp>
#include <Rendering/LightingView.hpp>
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
{
}

auto DepthBodyRenderer::render(
    BodySetView const & bodies,
    LightingView const & lightingView,
    DepthMapping & target) const
    -> void
{
    shader->use();

    auto const & spotLightingView = lightingView.getSpotView();

    auto & spotDepthMapping = target.getSpotMapping();

    for (auto i = 0u; i < spotLightingView.size(); ++i)
    {
        auto const & spotView = spotLightingView[i];

        auto & depthMap = spotDepthMapping[i];

        render(bodies, spotView, depthMap);
    }
}

auto DepthBodyRenderer::render(
    BodySetView const & bodies,
    Camera const & lightView,
    DepthMap & target) const
    -> void
{
    auto const binder = ScopedBinder{target.getFrameBuffer()};

    auto const mapSize = target.getSize();

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
