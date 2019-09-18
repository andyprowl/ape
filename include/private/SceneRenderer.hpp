#pragma once

#include "CameraUniform.hpp"
#include "LightingUniform.hpp"

class Scene;
class ShaderProgram;

class SceneRenderer
{

public:

    SceneRenderer(ShaderProgram const & shader, glm::vec3 const & backgroundColor);

    auto render(Scene const & s) const
        -> void;

private:

    class UniformSet
    {

    public:

        UniformSet(ShaderProgram const & shader)
            : camera{shader, "camera"}
            , lighting{shader, "lighting"}
        {
        }

    public:

        Uniform<Camera> camera;

        Uniform<Lighting> lighting;

    };

private:

    auto clear() const
        -> void;

    auto drawScene(Scene const & s) const
        -> void;

private:

    ShaderProgram const * shader;

    glm::vec3 backgroundColor;

    mutable UniformSet uniforms;

};