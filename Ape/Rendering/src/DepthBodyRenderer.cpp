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

auto const depthMapSize = Size<int>{1000, 500};

auto makeDepthMap()
    -> Texture
{
    auto const descriptor = TextureDescriptor{
        depthMapSize,
        TextureFormat::depthOnly,
        PixelType::floatingPoint,
        nullptr};

    return Texture{descriptor};
}

auto makeDepthFrameBuffer(Texture & depthMap)
    -> FrameBufferObject
{
    auto frameBuffer = FrameBufferObject{};

    auto const binder = ScopedBinder{frameBuffer};

    frameBuffer.attach(depthMap, FrameBufferAttachment::depth);

    frameBuffer.resetDrawTarget();

    frameBuffer.resetReadTarget();

    assert(frameBuffer.isComplete());

    return frameBuffer;
}

} // unnamed namespace

DepthBodyRenderer::DepthBodyRenderer(
    DepthShaderProgram & shader,
    ShapeRenderer const & shapeRenderer)
    : shader{&shader}
    , shapeRenderer{&shapeRenderer}
    , depthMap{makeDepthMap()}
    , depthFrameBuffer{makeDepthFrameBuffer(depthMap)}
{
}

auto DepthBodyRenderer::render(BodyRange const & bodies, Camera const & lightView) const
    -> void
{
    shader->use();

    auto const binder = ScopedBinder{depthFrameBuffer};

    glViewport(0, 0, depthMapSize.width, depthMapSize.height);

    glClear(GL_DEPTH_BUFFER_BIT);

    auto const & lightTransformation = lightView.getTransformation();

    for (auto const body : bodies)
    {
        renderBody(*body, lightTransformation);
    }

    glCullFace(GL_BACK);
}

auto DepthBodyRenderer::getDepthMap() const
    -> Texture const &
{
    return depthMap;
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
