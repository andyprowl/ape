#pragma once

#include <Ape/Skybox/PlainCube.hpp>

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

private:

    auto setupUniforms(Camera const & camera, CubeTexture const & skybox) const
        -> void;

    auto drawCube() const
        -> void;

private:

    SkyboxShaderProgram * shader;

    SkyboxSelector * selector;

    PlainCube skyboxShape;

};

} // namespace ape
