#include "AssetLoader.hpp"

#include "AssetRepository.hpp"
#include "MaterialLoader.hpp"
#include "MeshLoader.hpp"
#include "ModelLoader.hpp"

#include <assimp/scene.h>

#include <algorithm>

auto AssetLoader::load(aiScene const & scene, std::string const & directory) const
    -> AssetRepository
{
    auto repository = AssetRepository{};

    load(scene, directory, repository);

    return repository;
}

auto AssetLoader::load(
    aiScene const & scene,
    std::string const & directory,
    AssetRepository & target) const
    -> void
{
    importMaterials(scene, directory, target);

    importMeshes(scene, target);

    importModel(scene, target);
}

auto AssetLoader::importMaterials(
    aiScene const & scene,
    std::string const & directory,
    AssetRepository & target) const
    -> void
{
    auto & assets = target.getAssets();

    auto loader = MaterialLoader{assets};

    loader.load(scene, directory);
}

auto AssetLoader::importMeshes(aiScene const & scene, AssetRepository & target) const
    -> void
{
    auto & assets = target.getAssets();

    auto loader = MeshLoader{assets};

    loader.load(scene);
}

auto AssetLoader::importModel(aiScene const & scene, AssetRepository & target) const
    -> void
{
    auto loader = ModelLoader{target};

    loader.load(scene);
}
