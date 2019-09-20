#include "ModelLoader.hpp"

#include "AssetLoader.hpp"
#include "AssetRepository.hpp"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

namespace
{

auto makeMatrix(aiMatrix4x4 const & m)
    -> glm::mat4
{
    return {
        m[0][0], m[0][1], m[0][2], m[0][3],
        m[1][0], m[1][1], m[1][2], m[1][3],
        m[2][0], m[2][1], m[2][2], m[2][3],
        m[3][0], m[3][1], m[3][2], m[3][3]};
}

} // unnamed namespace

ModelLoader::ModelLoader(AssetRepository & target)
    : target{&target}
{
}

auto ModelLoader::load(aiScene const & scene) const
    -> void
{
    auto importer = Assimp::Importer{};

    auto rootPart = loadModelPart(*(scene.mRootNode), scene);

    auto & models = target->getAssets().models;

    models.emplace_back("", "", std::move(rootPart));
}

auto ModelLoader::loadModelPart(aiNode const & node, aiScene const & scene) const
    -> ModelPart
{
    auto components = std::vector<ModelPart>{};

    components.reserve(node.mNumChildren);

    for (auto i = 0u; i < node.mNumChildren; ++i)
    {
        auto const childNode = node.mChildren[i];

        auto childPart = loadModelPart(*childNode, scene);

        components.push_back(std::move(childPart));
    }

    auto const transformation = makeMatrix(node.mTransformation);

    auto meshes = importMeshes(node);

    return {node.mName.C_Str(), std::move(meshes), std::move(components), transformation};
}

auto ModelLoader::importMeshes(aiNode const & node) const
    -> std::vector<Mesh const *>
{
    auto const & meshes = target->getAssets().meshes;

    auto bodyMeshes = std::vector<Mesh const *>{};

    bodyMeshes.reserve(node.mNumMeshes);

    std::transform(
        node.mMeshes,
        node.mMeshes + node.mNumMeshes,
        std::back_inserter(bodyMeshes),
        [&meshes] (int const index)
    {
        return &meshes[index];
    });

    return bodyMeshes;
}
