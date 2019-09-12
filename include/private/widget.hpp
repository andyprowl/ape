#pragma once

#include "shape.hpp"

#include <glm/mat4x4.hpp>

#include <memory>
#include <vector>

class ShaderProgram;
class Shape;

class Widget
{

public:

    Widget(
        std::shared_ptr<Shape const> shape,
        std::vector<int> textureIds,
        ShaderProgram const & shaderProgram,
        glm::mat4 const & modelTransformation = glm::mat4{1.0f});

    auto draw() const
        -> void;

    auto getModelTransformation() const
        -> glm::mat4;
    
    auto setModelTransformation(glm::mat4 const & newTransformation)
        -> void;

    auto scaleUniformly(float factor)
        -> void;

    auto translate(glm::vec3 const & offset)
        -> void;

private:

    auto bindTextures() const
        -> void;

private:

    std::shared_ptr<Shape const> shape;

    std::vector<int> textureIds;

    ShaderProgram const * shaderProgram;

    glm::mat4 modelTransformation;

    glm::mat3 normalMatrix;

};
