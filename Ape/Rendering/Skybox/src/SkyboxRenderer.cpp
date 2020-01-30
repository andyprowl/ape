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

auto SkyboxRenderer::render(
    Camera const & camera,
    LightSystem const & lightSystem,
    Fog const & fog) const
    -> void
{
    auto const skybox = selector->getActiveSkybox();

    if (skybox == nullptr)
    {
        return;
    }

    auto const shaderBinder = bind(*shader);

    setupUniforms(camera, lightSystem, fog, *skybox);

    glDepthFunc(GL_LEQUAL);

    drawCube();

    glDepthFunc(GL_LESS);
}

auto SkyboxRenderer::setupUniforms(
    Camera const & camera,
    LightSystem const & lightSystem,
    Fog const & fog,
    glow::CubeTexture const & skybox) const
    -> void
{
    auto const projection = camera.getProjection().getTransformation();

    auto const viewRotation = glm::mat4{glm::mat3{camera.getView().getTransformation()}};

    shader->lightSystem.set(lightSystem);

    shader->transform.set(projection * viewRotation);

    shader->skybox.set(skybox);

    shader->cameraPosition.set(camera.getView().getPosition());

    shader->fog.set(fog);
}

auto SkyboxRenderer::drawCube() const
    -> void
{
    auto const arrayObjectBinder = glow::bind(skyboxShape.getArrayObject());

    auto const numOfVertices = skyboxShape.getNumOfVertices();

    glDrawArrays(GL_TRIANGLES, 0, numOfVertices);
}

} // namespace ape
