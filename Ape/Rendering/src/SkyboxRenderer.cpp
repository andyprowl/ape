#include <Ape/Rendering/SkyboxRenderer.hpp>

#include <Ape/Rendering/SkyboxSelector.hpp>
#include <Ape/Rendering/SkyboxShaderProgram.hpp>

#include <Ape/GpuResource/ScopedBinder.hpp>
#include <Ape/Scene/Camera.hpp>

#include <glad/glad.h>

namespace ape
{

SkyboxRenderer::SkyboxRenderer(SkyboxShaderProgram & shader, SkyboxSelector & selector)
    : shader{&shader}
    , selector{&selector}
    , viewport{{0, 0}, {0, 0}}
{
}

auto SkyboxRenderer::render(Camera const & camera) const
    -> void
{
    const auto skybox = selector->getActiveSkybox();

    if (skybox == nullptr)
    {
        return;
    }

    auto const shaderBinder = bind(*shader);

    setupViewport();

    setupUniforms(camera, *skybox);

    glDepthFunc(GL_LEQUAL);

    drawCube();

    glDepthFunc(GL_LESS);
}

auto SkyboxRenderer::setViewport(Viewport const & newViewport)
    -> void
{
    viewport = newViewport;
}

auto SkyboxRenderer::setupViewport() const
    -> void
{
    glViewport(viewport.origin.x, viewport.origin.y, viewport.size.width, viewport.size.height);
}

auto SkyboxRenderer::setupUniforms(Camera const & camera, CubeTexture const & skybox) const
    -> void
{
    auto const projection = camera.getProjection().getTransformation();

    auto const viewRotation = glm::mat4{glm::mat3{camera.getView().getTransformation()}};

    shader->transform = projection * viewRotation;

    shader->skybox = skybox;
}

auto SkyboxRenderer::drawCube() const
    -> void
{
    auto const arrayObjectBinder = bind(skyboxShape.getArrayObject());

    auto const numOfVertices = skyboxShape.getNumOfVertices();

    glDrawArrays(GL_TRIANGLES, 0, numOfVertices);
}

} // namespace ape
