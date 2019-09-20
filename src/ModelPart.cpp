#include "ModelPart.hpp"

#include <glm/gtc/matrix_transform.hpp>

ModelPart::ModelPart(
    std::string name,
    std::vector<Mesh const *> meshes,
    std::vector<ModelPart> components,
    glm::mat4 const & transformation)
    : name{std::move(name)}
    , meshes{std::move(meshes)}
    , components{std::move(components)}
    , transformation{std::move(transformation)}
{
}

auto ModelPart::getName() const
    -> std::string
{
    return name;
}

auto ModelPart::getMeshes() const
    -> std::vector<Mesh const *> const &
{
    return meshes;
}

auto ModelPart::getComponents() const
    -> std::vector<ModelPart> const &
{
    return components;
}

auto ModelPart::getTransformation() const
    -> glm::mat4 const &
{
    return transformation;
}
