#pragma once

#include "Material.hpp"
#include "Shape.hpp"
#include "Uniform.hpp"

#include <glm/mat4x4.hpp>

#include <memory>
#include <vector>

class ShaderProgram;
class Shape;

class Mesh
{

public:

    Mesh(
        std::shared_ptr<Shape const> shape,
        ShaderProgram const & shader,
        Material const & material,
        glm::mat4 const & modelTransformation = glm::mat4{1.0f});

    auto draw() const
        -> void;

    auto getPosition() const
        -> glm::vec3;

    auto getModelTransformation() const
        -> glm::mat4;
    
    auto setModelTransformation(glm::mat4 const & newTransformation)
        -> void;

    auto scaleUniformly(float factor)
        -> void;

    auto translate(glm::vec3 const & offset)
        -> void;

    auto getMaterial() const
        -> Material;

private:

    class UniformSet
    {

    public:

        UniformSet(ShaderProgram const & program)
            : modelTransformation{program, "transform.model"}
            , normalMatrix{program, "transform.normal"}
            , materialAmbient{program, "material.ambient"}
            , materialShininess{program, "material.shininess"}
        {
        }
    
    public:
    
        Uniform<glm::mat4> modelTransformation;

        Uniform<glm::mat3> normalMatrix;

        Uniform<glm::vec3> materialAmbient;

        Uniform<float> materialShininess;

    };

private:

    auto setTransformationsInShader() const
        -> void;

    auto setMaterialInShader() const
        -> void;

    auto drawShape() const
        -> void;

private:

    std::shared_ptr<Shape const> shape;

    ShaderProgram const * shader;

    Material material;

    glm::mat4 modelTransformation;

    glm::mat3 normalMatrix;

    mutable UniformSet uniforms;

};
