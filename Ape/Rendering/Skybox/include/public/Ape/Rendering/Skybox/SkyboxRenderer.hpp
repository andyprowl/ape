#pragma once

#include <Ape/Rendering/Skybox/PlainCube.hpp>

namespace glow
{

class TextureCube;

} // namespace glow

namespace ape
{

class Camera;
class Fog;
class SkyboxSelector;
class SkyboxShaderProgram;

class SkyboxRenderer
{

public:

    SkyboxRenderer(SkyboxShaderProgram & shader, SkyboxSelector & selector);

    auto render(Camera const & camera, Fog const & fog) const
        -> void;

private:

    auto setupUniforms(
        Camera const & camera,
        Fog const & fog,
        glow::TextureCube const & skybox) const
        -> void;

    auto drawCube() const
        -> void;

private:

    SkyboxShaderProgram * shader;

    SkyboxSelector * selector;

    PlainCube skyboxShape;

};

} // namespace ape
