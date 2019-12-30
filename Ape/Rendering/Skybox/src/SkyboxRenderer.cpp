#include <Ape/Rendering/Skybox/SkyboxRenderer.hpp>

#include <Ape/Rendering/Skybox/SkyboxSelector.hpp>
#include <Ape/Rendering/Skybox/SkyboxShaderProgram.hpp>

#include <Ape/World/Scene/Camera.hpp>

#include <Glow/GpuResource/ScopedBinder.hpp>

#include <glad/glad.h>

namespace ape
{

SkyboxRenderer::SkyboxRenderer(SkyboxShaderProgram & shader, SkyboxSelector & selector)
    : shader{&shader}
    , selector{&selector}
{
}

auto SkyboxRenderer::render(Camera const & camera) const
    -> void
{
    auto const skybox = selector->getActiveSkybox();

    if (skybox == nullptr)
    {
        return;
    }

    auto const shaderBinder = bind(*shader);

    setupUniforms(camera, *skybox);

    glDepthFunc(GL_LEQUAL);

    drawCube();

    glDepthFunc(GL_LESS);
}

auto SkyboxRenderer::setupUniforms(Camera const & camera, glow::CubeTexture const & skybox) const
    -> void
{
    auto const projection = camera.getProjection().getTransformation();

    auto const viewRotation = glm::mat4{glm::mat3{camera.getView().getTransformation()}};

    shader->transform = projection * viewRotation;

    shader->skybox = skybox;

    shader->cameraHeight = camera.getView().getPosition().y;
}

auto SkyboxRenderer::drawCube() const
    -> void
{
    auto const arrayObjectBinder = glow::bind(skyboxShape.getArrayObject());

    auto const numOfVertices = skyboxShape.getNumOfVertices();

    glDrawArrays(GL_TRIANGLES, 0, numOfVertices);
}

} // namespace ape
