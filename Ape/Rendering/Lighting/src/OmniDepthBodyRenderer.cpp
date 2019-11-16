#include <Ape/Rendering/Lighting/OmniDepthBodyRenderer.hpp>

#include <Ape/Rendering/Lighting/DepthMapping.hpp>
#include <Ape/Rendering/Lighting/LightSystemView.hpp>
#include <Ape/Rendering/Lighting/OmniDepthShaderProgram.hpp>

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

OmniDepthBodyRenderer::OmniDepthBodyRenderer(
    OmniDepthShaderProgram & shader,
    ShapeDrawer const & shapeRenderer)
    : shader{&shader}
    , shapeRenderer{&shapeRenderer}
{
}

auto OmniDepthBodyRenderer::render(
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
    auto const & worldTransformation = part.getWorldTransformation();
    
    shader->worldTransformation = worldTransformation;

    shader->lightTransformation = lightTransformation * worldTransformation;

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
