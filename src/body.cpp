#include "body.hpp"

#include "shader.hpp"
#include "shape.hpp"

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

Body::Body(
    std::shared_ptr<Shape const> shape,
    Material const & material,
    ShaderProgram const & shaderProgram,
    glm::mat4 const & modelTransformation)
    : shape{std::move(shape)}
    , material{material}
    , shaderProgram{&shaderProgram}
    , modelTransformation{modelTransformation}
    , normalMatrix{computeNormalMatrix(modelTransformation)}
{
}

auto Body::draw() const
    -> void
{
    shaderProgram->use();

    setTransformationsInShader();

    setMaterialInShader();

    drawShape();
}

auto Body::getPosition() const
    -> glm::vec3
{
    return glm::vec3{modelTransformation[3]};
}

auto Body::getModelTransformation() const
    -> glm::mat4
{
    return modelTransformation;
}

auto Body::setModelTransformation(glm::mat4 const & newTransformation)
    -> void
{
    modelTransformation = newTransformation;

    normalMatrix = computeNormalMatrix(modelTransformation);
}

auto Body::scaleUniformly(float const factor)
    -> void
{
    modelTransformation = glm::scale(modelTransformation, glm::vec3{factor, factor, factor});
}

auto Body::translate(glm::vec3 const & offset)
    -> void
{
    modelTransformation = glm::translate(modelTransformation, offset);
}

auto Body::getMaterial() const
    -> Material
{
    return material;
}

auto Body::setTransformationsInShader() const
    -> void
{
    shaderProgram->set("transform.model", modelTransformation);

    shaderProgram->set("transform.normal", normalMatrix);
}

auto Body::setMaterialInShader() const
    -> void
{
    shaderProgram->set("material.ambient", material.ambient);

    shaderProgram->set("material.shininess", material.shininess);

    glActiveTexture(GL_TEXTURE0);

    glBindTexture(GL_TEXTURE_2D, material.diffuseMapId);

    glActiveTexture(GL_TEXTURE1);

    glBindTexture(GL_TEXTURE_2D, material.specularMapId);
}

auto Body::drawShape() const
    -> void
{
    shape->draw();
}
