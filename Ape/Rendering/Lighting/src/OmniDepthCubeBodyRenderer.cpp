#include <Ape/Rendering/Lighting/OmniDepthCubeBodyRenderer.hpp>

#include <Ape/Rendering/Lighting/DepthMapping.hpp>
#include <Ape/Rendering/Lighting/LightSystemView.hpp>
#include <Ape/Rendering/Lighting/OmniDepthCubeShaderProgram.hpp>

#include <Ape/World/Model/Mesh.hpp>
#include <Ape/World/Model/ModelPart.hpp>
#include <Ape/World/Scene/Body.hpp>
#include <Ape/World/Scene/BodyPart.hpp>
#include <Ape/World/Scene/Camera.hpp>
#include <Ape/World/Scene/LightSystem.hpp>
#include <Ape/World/Shape/ShapeDrawer.hpp>

#include <Glow/GpuResource/ScopedBinder.hpp>

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

OmniDepthCubeBodyRenderer::OmniDepthCubeBodyRenderer(
    OmniDepthCubeShaderProgram & shader,
    ShapeDrawer const & shapeRenderer)
    : shader{&shader}
    , shapeRenderer{&shapeRenderer}
{
}

auto OmniDepthCubeBodyRenderer::render(
    BodySetView const & bodies,
    LightSystemView const & lightSystemView,
    DepthMapping & target) const
    -> void
{
    auto const shaderBinder = bind(*shader);
    
    renderLightSetDepth(
        bodies,
        lightSystemView.getLighting().point,
        lightSystemView.getPointView(),
        target.getPointMapping());
}

auto OmniDepthCubeBodyRenderer::renderLightSetDepth(
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

auto OmniDepthCubeBodyRenderer::renderLightDepth(
    BodySetView const & bodies,
    PointLightView const & lightView,
    OmniDepthMap & target) const
    -> void
{
    auto const binder = bind(target.getFrameBuffer());

    auto const mapSize = target.getSize();

    glViewport(0, 0, mapSize.width, mapSize.height);

    glClear(GL_DEPTH_BUFFER_BIT);

    for (auto const & body : bodies)
    {
        renderBody(asReference(body), lightView);
    }
}

auto OmniDepthCubeBodyRenderer::renderBody(
    Body const & body,
    PointLightView const & lightView) const
    -> void
{
    for (auto const & part : body.getParts())
    {
        renderBodyPart(part, lightView);
    }
}

auto OmniDepthCubeBodyRenderer::renderBodyPart(
    BodyPart const & part,
    PointLightView const & lightView) const
    -> void
{
    auto const & worldTransformation = part.getWorldTransformation();
    
    shader->worldTransformation.set(worldTransformation);

    auto const lightTransformation = getTransformation(lightView);

    shader->lightTransformation = lightTransformation * worldTransformation;

    for (auto const mesh : part.getModel().getMeshes())
    {
        renderMesh(*mesh);
    }
}

auto OmniDepthCubeBodyRenderer::renderMesh(Mesh const & mesh) const
    -> void
{
    auto const & shape = mesh.getShape();
    
    shapeRenderer->render(shape);
}

} // namespace ape
