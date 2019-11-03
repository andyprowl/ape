#pragma once

#include <Ape/Rendering/PlainCube.hpp>
#include <Ape/Rendering/Viewport.hpp>

namespace ape
{

class Camera;
class CubeTexture;
class SkyboxSelector;
class SkyboxShaderProgram;

class SkyboxRenderer
{

public:

    SkyboxRenderer(SkyboxShaderProgram & shader, SkyboxSelector & selector);

    auto render(Camera const & camera) const
        -> void;

    auto setViewport(Viewport const & newViewport)
        -> void;

private:

    auto setupViewport() const
        -> void;

    auto setupUniforms(Camera const & camera, CubeTexture const & skybox) const
        -> void;

    auto drawCube() const
        -> void;

private:

    SkyboxShaderProgram * shader;

    SkyboxSelector * selector;

    PlainCube skyboxShape;

    Viewport viewport;

};

} // namespace ape
