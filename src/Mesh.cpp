#include "Mesh.hpp"

#include "ShaderProgram.hpp"
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

auto Mesh::getShape() const
    -> Shape const &
{
    return *shape;
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

auto Mesh::getNormalMatrix() const
    -> glm::mat3
{
    return normalMatrix;
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
