#pragma once

#include <Ape/Rendering/Skybox/PlainCube.hpp>

namespace glow
{

class CubeTexture;

} // namespace glow

namespace ape
{

class Camera;
class LightSystem;
class SkyboxSelector;
class SkyboxShaderProgram;

class SkyboxRenderer
{

public:

    SkyboxRenderer(SkyboxShaderProgram & shader, SkyboxSelector & selector);

    auto render(Camera const & camera, LightSystem const & lightSystem) const
        -> void;

private:

    auto setupUniforms(
        Camera const & camera,
        LightSystem const & lightSystem,
        glow::CubeTexture const & skybox) const
        -> void;

    auto drawCube() const
        -> void;

private:

    SkyboxShaderProgram * shader;

    SkyboxSelector * selector;

    PlainCube skyboxShape;

};

} // namespace ape
