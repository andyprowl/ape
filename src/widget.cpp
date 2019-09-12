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
    ShaderProgram const & shaderProgram,
    glm::mat4 const & modelTransformation)
    : shape{std::move(shape)}
    , textureIds{std::move(textureIds)}
    , shaderProgram{&shaderProgram}
    , modelTransformation{modelTransformation}
    , normalMatrix{computeNormalMatrix(modelTransformation)}
{
}

auto Widget::draw() const
    -> void
{
    bindTextures();

    shaderProgram->use();

    shaderProgram->set("model", modelTransformation);

    shaderProgram->set("normalMatrix", normalMatrix);

    shape->draw();
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

auto Widget::bindTextures() const
    -> void
{
    for (auto i = 0; i < static_cast<int>(textureIds.size()); ++i)
    {
        glActiveTexture(GL_TEXTURE0 + i);

        glBindTexture(GL_TEXTURE_2D, textureIds[i]);
    }
}
