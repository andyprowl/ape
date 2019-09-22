#include <Ape/ModelPart.hpp>

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
    , parent{nullptr}
    , inDepthIndex{0}
{
    updateDescendantsInDepthIndex();
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

auto ModelPart::setSelfAsComponentParent()
    -> void
{
    for (auto & component : components)
    {
        component.parent = this;
    }
}

auto ModelPart::updateDescendantsInDepthIndex()
    -> void
{
    auto i = 1;

    for (auto & component : components)
    {
        component.inDepthIndex = inDepthIndex + i;

        component.updateDescendantsInDepthIndex();

        ++i;
    }
}

auto ModelPart::getTransformation() const
    -> glm::mat4 const &
{
    return transformation;
}

auto ModelPart::getParent() const
    -> ModelPart const *
{
    return parent;
}

auto ModelPart::getInDepthIndex() const
    -> int
{
    return inDepthIndex;
}

auto isRoot(ModelPart const & part)
    -> bool
{
    return (part.getParent() == nullptr);
}
