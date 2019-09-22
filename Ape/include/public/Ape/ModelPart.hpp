#pragma once

#include <Ape/Mesh.hpp>

#include <glm/mat4x4.hpp>

#include <vector>

class ModelPart
{

public:

    ModelPart(
        std::string name,
        std::vector<Mesh const *> meshes,
        std::vector<ModelPart> components,
        glm::mat4 const & localTransformation);

    auto getName() const
        -> std::string;

    auto getMeshes() const
        -> std::vector<Mesh const *> const &;

    auto getComponents() const
        -> std::vector<ModelPart> const &;

    auto getTransformation() const
        -> glm::mat4 const &;

    auto getParent() const
        -> ModelPart const *;

    auto getInDepthIndex() const
        -> int;

private:

    auto setSelfAsComponentParent()
        -> void;

    auto updateDescendantsInDepthIndex()
        -> void;

private:

    std::string name;

    std::vector<Mesh const *> meshes;

    std::vector<ModelPart> components;

    glm::mat4 transformation;

    ModelPart const * parent;

    int inDepthIndex;

};

auto isRoot(ModelPart const & part)
    -> bool;
