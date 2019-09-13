#pragma once

#include "material.hpp"
#include "shape.hpp"

#include <glm/mat4x4.hpp>

#include <memory>
#include <vector>

class ShaderProgram;
class Shape;

class Body
{

public:

    Body(
        std::shared_ptr<Shape const> shape,
        Material const & material,
        ShaderProgram const & shaderProgram,
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

    auto setTransformationsInShader() const
        -> void;

    auto setMaterialInShader() const
        -> void;

    auto drawShape() const
        -> void;

private:

    std::shared_ptr<Shape const> shape;

    Material material;

    ShaderProgram const * shaderProgram;

    glm::mat4 modelTransformation;

    glm::mat3 normalMatrix;

};
