#include "widget.hpp"

#include "shader.hpp"
#include "shape.hpp"

#include "glad/glad.h"

#include <glm/gtc/matrix_transform.hpp>

Widget::Widget(
    std::shared_ptr<Shape const> shape,
    std::vector<int> textureIds,
    ShaderProgram const & shaderProgram,
    glm::mat4 const & modelTransformation)
    : shape{std::move(shape)}
    , textureIds{std::move(textureIds)}
    , shaderProgram{&shaderProgram}
    , modelTransformation{modelTransformation}
{
}

auto Widget::draw() const
    -> void
{
    bindTextures();

    shaderProgram->use();

    shaderProgram->set("model", modelTransformation);

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

auto rotateWidgetAroundX(Widget & widget, float const radians)
    -> void
{
    auto const rot = glm::rotate(
        widget.getModelTransformation(),
        radians,
        glm::vec3{1.0f, 0.0f, 0.0f});
    
    widget.setModelTransformation(rot);
}

auto scaleWidget(Widget & widget, float const factor)
    -> void
{
    auto const scaling = glm::scale(
        widget.getModelTransformation(),
        glm::vec3{factor, factor, factor});
    
    widget.setModelTransformation(scaling);
}
