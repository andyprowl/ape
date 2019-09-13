#include "widget.hpp"

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

Widget::Widget(
    std::shared_ptr<Shape const> shape,
    std::vector<int> textureIds,
    Material const & material,
    ShaderProgram const & shaderProgram,
    glm::mat4 const & modelTransformation)
    : shape{std::move(shape)}
    , textureIds{std::move(textureIds)}
    , material{material}
    , shaderProgram{&shaderProgram}
    , modelTransformation{modelTransformation}
    , normalMatrix{computeNormalMatrix(modelTransformation)}
{
}

auto Widget::draw() const
    -> void
{
    shaderProgram->use();

    bindTextures();

    setTransformationsInShader();

    setMaterialInShader();

    drawShape();
}

auto Widget::getPosition() const
    -> glm::vec3
{
    return glm::vec3{modelTransformation[3]};
}

auto Widget::getModelTransformation() const
    -> glm::mat4
{
    return modelTransformation;
}

auto Widget::setModelTransformation(glm::mat4 const & newTransformation)
    -> void
{
    modelTransformation = newTransformation;

    normalMatrix = computeNormalMatrix(modelTransformation);
}

auto Widget::scaleUniformly(float const factor)
    -> void
{
    modelTransformation = glm::scale(modelTransformation, glm::vec3{factor, factor, factor});
}

auto Widget::translate(glm::vec3 const & offset)
    -> void
{
    modelTransformation = glm::translate(modelTransformation, offset);
}

auto Widget::getMaterial() const
    -> Material
{
    return material;
}

auto Widget::bindTextures() const
    -> void
{
    for (auto i = 0; i < static_cast<int>(textureIds.size()); ++i)
    {
        glActiveTexture(GL_TEXTURE0 + i);

        glBindTexture(GL_TEXTURE_2D, textureIds[i]);
    }
}

auto Widget::setTransformationsInShader() const
    -> void
{
    shaderProgram->set("transform.model", modelTransformation);

    shaderProgram->set("transform.normal", normalMatrix);
}

auto Widget::setMaterialInShader() const
    -> void
{
    shaderProgram->set("material.ambient", material.ambient);

    shaderProgram->set("material.diffuse", material.diffuse);

    shaderProgram->set("material.specular", material.specular);

    shaderProgram->set("material.shininess", material.shininess);
}

auto Widget::drawShape() const
    -> void
{
    shape->draw();
}
