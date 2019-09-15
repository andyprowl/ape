#include "Mesh.hpp"

#include "Shader.hpp"
#include "Shape.hpp"

#include "glad/glad.h"

#include <glm/gtc/matrix_transform.hpp>

namespace
{

auto computeNormalMatrix(glm::mat4 const & model)
    -> glm::mat3
{
    return glm::mat3{glm::transpose(glm::inverse(model))};
}

} // unnamed namespace

Mesh::Mesh(
    std::shared_ptr<Shape const> shape,
    Material const & material,
    glm::mat4 const & modelTransformation)
    : shape{std::move(shape)}
    , material{material}
    , modelTransformation{modelTransformation}
    , normalMatrix{computeNormalMatrix(modelTransformation)}
{
}

auto Mesh::draw(ShaderProgram const & shaderProgram) const
    -> void
{
    shaderProgram.use();

    setTransformationsInShader(shaderProgram);

    setMaterialInShader(shaderProgram);

    drawShape();
}

auto Mesh::getPosition() const
    -> glm::vec3
{
    return glm::vec3{modelTransformation[3]};
}

auto Mesh::getModelTransformation() const
    -> glm::mat4
{
    return modelTransformation;
}

auto Mesh::setModelTransformation(glm::mat4 const & newTransformation)
    -> void
{
    modelTransformation = newTransformation;

    normalMatrix = computeNormalMatrix(modelTransformation);
}

auto Mesh::scaleUniformly(float const factor)
    -> void
{
    modelTransformation = glm::scale(modelTransformation, glm::vec3{factor, factor, factor});
}

auto Mesh::translate(glm::vec3 const & offset)
    -> void
{
    modelTransformation = glm::translate(modelTransformation, offset);
}

auto Mesh::getMaterial() const
    -> Material
{
    return material;
}

auto Mesh::setTransformationsInShader(ShaderProgram const & shaderProgram) const
    -> void
{
    shaderProgram.set("transform.model", modelTransformation);

    shaderProgram.set("transform.normal", normalMatrix);
}

auto Mesh::setMaterialInShader(ShaderProgram const & shaderProgram) const
    -> void
{
    shaderProgram.set("material.ambient", material.ambient);

    shaderProgram.set("material.shininess", material.shininess);

    material.diffuseMap.bind(0);

    material.specularMap.bind(1);
}

auto Mesh::drawShape() const
    -> void
{
    shape->draw();
}
