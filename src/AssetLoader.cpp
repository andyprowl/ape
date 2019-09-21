#include "AssetLoader.hpp"

#include <assimp/Importer.hpp>

#include "AssetRepository.hpp"
#include "MaterialLoader.hpp"
#include "MeshLoader.hpp"
#include "ModelLoader.hpp"

#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <algorithm>

namespace
{

auto extractDirectory(std::string const & path)
    -> std::string
{
    auto const lastSlashIndex = path.find_last_of("\\/");

    if (lastSlashIndex == std::string::npos)
    {
        return path;
    }

    return path.substr(0, lastSlashIndex);
}

} // unnamed namespace

auto AssetLoader::load(std::string const & path) const
    -> AssetRepository
{
    auto importer = Assimp::Importer{};

    const auto scene = importer.ReadFile(path, aiProcess_Triangulate /*| aiProcess_FlipUVs*/); 

    if (scene == nullptr)
    {
        throw CouldNotLoadAssets(path, importer.GetErrorString());
    }

    return load(path, *scene);
}

auto AssetLoader::load(std::string const & source, aiScene const & scene) const
    -> AssetRepository
{
    auto repository = AssetRepository{};

    load(source, scene, repository);

    return repository;
}

auto AssetLoader::load(
    std::string const & source,
    aiScene const & scene,
    AssetRepository & target) const
    -> void
{
    importMaterials(source, scene, target);

    importMeshes(scene, target);

    importModel(source, scene, target);
}

auto AssetLoader::importMaterials(
    std::string const & source,
    aiScene const & scene,
    AssetRepository & target) const
    -> void
{
    auto loader = MaterialLoader{target};

    auto const directory = extractDirectory(source);

    loader.load(scene, directory);
}

auto AssetLoader::importMeshes(aiScene const & scene, AssetRepository & target) const
    -> void
{
    auto loader = MeshLoader{target};

    loader.load(scene);
}

auto AssetLoader::importModel(
    std::string const & /*source*/,
    aiScene const & scene,
    AssetRepository & target) const
    -> void
{
    auto loader = ModelLoader{target};

    loader.load(scene);
}
