#include <Ape/Rendering/MonoDepthBodyRenderer.hpp>

#include <Ape/Rendering/DepthMapping.hpp>
#include <Ape/Rendering/MonoDepthShaderProgram.hpp>
#include <Ape/Rendering/LightingView.hpp>
#include <Ape/Rendering/ShapeRenderer.hpp>

#include <Ape/GpuResource/ScopedBinder.hpp>
#include <Ape/Model/Mesh.hpp>
#include <Ape/Model/ModelPart.hpp>
#include <Ape/Scene/Body.hpp>
#include <Ape/Scene/BodyPart.hpp>
#include <Ape/Scene/Camera.hpp>
#include <Ape/Scene/Lighting.hpp>

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

MonoDepthBodyRenderer::MonoDepthBodyRenderer(
    MonoDepthShaderProgram & shader,
    ShapeRenderer const & shapeRenderer)
    : shader{&shader}
    , shapeRenderer{&shapeRenderer}
{
}

auto MonoDepthBodyRenderer::render(
    BodySetView const & bodies,
    LightingView const & lightingView,
    DepthMapping & target) const
    -> void
{
    auto const shaderBinder = bind(*shader);

    renderSpotLightSetDepth(bodies, lightingView, target);

    renderDirectionalLightSetDepth(bodies, lightingView, target);
}

auto MonoDepthBodyRenderer::renderSpotLightSetDepth(
    BodySetView const & bodies,
    LightingView const & lightingView,
    DepthMapping & target) const
    -> void
{
    auto const & lighting = lightingView.getLighting();

    auto const & spotView = lightingView.getSpotView();

    auto & spotMapping = target.getSpotMapping();

    renderLightSetDepth(bodies, lighting.spot, spotView, spotMapping);
}

auto MonoDepthBodyRenderer::renderDirectionalLightSetDepth(
    BodySetView const & bodies,
    LightingView const & lightingView,
    DepthMapping & target) const
    -> void
{
    auto const & lighting = lightingView.getLighting();

    auto const & directionalView = lightingView.getDirectionalView();

    auto & directionalMapping = target.getDirectionalMapping();

    renderLightSetDepth(bodies, lighting.directional, directionalView, directionalMapping);
}

template<typename LightType>
auto MonoDepthBodyRenderer::renderLightSetDepth(
    BodySetView const & bodies,
    std::vector<LightType> const & lights,
    std::vector<glm::mat4> const & lightViews,
    std::vector<MonoDepthMap> & depthMaps) const
    -> void
{
    for (auto i = 0u; i < lights.size(); ++i)
    {
        if (!lights[i].isTurnedOn())
        {
            continue;
        }

        auto const & lightView = lightViews[i];

        auto & depthMap = depthMaps[i];

        renderLightDepth(bodies, lightView, depthMap);
    }
}

auto MonoDepthBodyRenderer::renderLightDepth(
    BodySetView const & bodies,
    glm::mat4 const & lightTransformation,
    MonoDepthMap & target) const
    -> void
{
    auto const binder = bind(target.getFrameBuffer());

    auto const mapSize = target.getSize();

    glViewport(0, 0, mapSize.width, mapSize.height);

    glClear(GL_DEPTH_BUFFER_BIT);

    for (auto const & body : bodies)
    {
        renderBody(asReference(body), lightTransformation);
    }
}

auto MonoDepthBodyRenderer::renderBody(
    Body const & body,
    glm::mat4 const & lightTransformation) const
    -> void
{
    for (auto const & part : body.getParts())
    {
        renderBodyPart(part, lightTransformation);
    }
}

auto MonoDepthBodyRenderer::renderBodyPart(
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

auto MonoDepthBodyRenderer::renderMesh(Mesh const & mesh) const
    -> void
{
    auto const & shape = mesh.getShape();
    
    shapeRenderer->render(shape);
}

} // namespace ape
