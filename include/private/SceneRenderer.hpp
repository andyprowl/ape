#pragma once

#include "CameraUniform.hpp"
#include "LightingUniform.hpp"

class ModelInstance;
class ModelPartInstance;
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
            , modelTransformation{shader, "model.transform"}
            , normalMatrix{shader, "model.normal"}
            , materialAmbient{shader, "material.ambient"}
            , materialShininess{shader, "material.shininess"}
        {
        }

    public:

        Uniform<Camera> camera;

        Uniform<Lighting> lighting;

        Uniform<glm::mat4> modelTransformation;

        Uniform<glm::mat3> normalMatrix;

        Uniform<glm::vec3> materialAmbient;

        Uniform<float> materialShininess;

    };

private:

    auto clear() const
        -> void;

    auto drawScene(Scene const & scene) const
        -> void;

    auto drawBody(ModelInstance const & body) const
        -> void;

    auto drawBodyPart(ModelPartInstance const & part) const
        -> void;

    auto drawMesh(Mesh const & mesh) const
        -> void;

private:

    ShaderProgram const * shader;

    glm::vec3 backgroundColor;

    mutable UniformSet uniforms;

};