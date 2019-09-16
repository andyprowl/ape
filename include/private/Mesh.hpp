#pragma once

#include "Material.hpp"
#include "Shape.hpp"

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
        Material const & material,
        glm::mat4 const & modelTransformation = glm::mat4{1.0f});

    auto draw(ShaderProgram const & shaderProgram) const
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

    auto setTransformationsInShader(ShaderProgram const & shaderProgram) const
        -> void;

    auto setMaterialInShader(ShaderProgram const & shaderProgram) const
        -> void;

    auto drawShape() const
        -> void;

private:

    std::shared_ptr<Shape const> shape;

    Material material;

    glm::mat4 modelTransformation;

    glm::mat3 normalMatrix;

};
