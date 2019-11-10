#include <Ape/Rendering/OmniDepthBodyRenderer.hpp>

#include <Ape/Rendering/DepthMapping.hpp>
#include <Ape/Rendering/LightingView.hpp>
#include <Ape/Rendering/OmniDepthShaderProgram.hpp>
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

OmniDepthBodyRenderer::OmniDepthBodyRenderer(
    OmniDepthShaderProgram & shader,
    ShapeRenderer const & shapeRenderer)
    : shader{&shader}
    , shapeRenderer{&shapeRenderer}
{
}

auto OmniDepthBodyRenderer::render(
    BodySetView const & bodies,
    LightingView const & lightingView,
    DepthMapping & target) const
    -> void
{
    auto const shaderBinder = bind(*shader);
    
    renderLightSetDepth(
        bodies,
        lightingView.getLighting().point,
        lightingView.getPointView(),
        target.getPointMapping());
}

auto OmniDepthBodyRenderer::renderLightSetDepth(
    BodySetView const & bodies,
    std::vector<PointLight> const & lights,
    std::vector<PointLightView> const & lightViews,
    std::vector<OmniDepthMap> & depthMaps) const
    -> void
{
    for (auto i = 0u; i < lights.size(); ++i)
    {
        if (!lights[i].isTurnedOn())
        {
            continue;
        }

        shader->lightPosition = lights[i].getPosition();

        auto const & lightView = lightViews[i];

        auto & depthMap = depthMaps[i];

        renderLightDepth(bodies, lightView, depthMap);
    }
}

auto OmniDepthBodyRenderer::renderLightDepth(
    BodySetView const & bodies,
    PointLightView const & lightTransformation,
    OmniDepthMap & target) const
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

auto OmniDepthBodyRenderer::renderBody(
    Body const & body,
    PointLightView const & lightTransformation) const
    -> void
{
    for (auto const & part : body.getParts())
    {
        renderBodyPart(part, lightTransformation);
    }
}

auto OmniDepthBodyRenderer::renderBodyPart(
    BodyPart const & part,
    PointLightView const & lightTransformation) const
    -> void
{
    auto const & modelTransformation = part.getGlobalTransformation();
    
    shader->modelTransformation = modelTransformation;

    shader->lightTransformation = lightTransformation * modelTransformation;

    for (auto const mesh : part.getModel().getMeshes())
    {
        renderMesh(*mesh);
    }
}

auto OmniDepthBodyRenderer::renderMesh(Mesh const & mesh) const
    -> void
{
    auto const & shape = mesh.getShape();
    
    shapeRenderer->render(shape);
}

} // namespace ape
