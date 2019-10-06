#include <AssetLoader/AssetLoader.hpp>

#include <AssetLoader/AssetRepository.hpp>
#include <AssetLoader/MaterialLoader.hpp>
#include <AssetLoader/MeshLoader.hpp>
#include <AssetLoader/ModelPartImporter.hpp>
#include <AssetLoader/TextureCache.hpp>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <algorithm>

namespace ape
{

auto AssetLoader::load(std::filesystem::path const & path, std::string modelName) const
    -> AssetRepository
{
    auto textureCache = TextureCache{};

    return load(path, textureCache, modelName);
}

auto AssetLoader::load(
    std::filesystem::path const & path,
    TextureCache & textureCache,
    std::string modelName) const
    -> AssetRepository
{
    auto importer = Assimp::Importer{};

    const auto scene = importer.ReadFile(path.string(), aiProcess_Triangulate); 

    if (scene == nullptr)
    {
        throw CouldNotLoadAssets(path, importer.GetErrorString());
    }

    auto const searchPath = path.parent_path();

    return load(*scene, searchPath, textureCache, std::move(modelName));
}

auto AssetLoader::load(
    aiScene const & scene,
    std::filesystem::path const & searchPath,
    TextureCache & textureCache,
    std::string modelName) const
    -> AssetRepository
{
    auto repository = AssetRepository{};

    load(scene, searchPath, textureCache, repository, std::move(modelName));

    return repository;
}

auto AssetLoader::load(
    aiScene const & scene,
    std::filesystem::path const & searchPath,
    TextureCache & textureCache,
    AssetRepository & target,
    std::string modelName) const
    -> void
{
    importMaterials(scene, searchPath, textureCache, target);

    importMeshes(scene, target);

    importModel(scene, target, std::move(modelName));
}

auto AssetLoader::importMaterials(
    aiScene const & scene,
    std::filesystem::path const & searchPath,
    TextureCache & textureCache,
    AssetRepository & target) const
    -> void
{
    auto const loader = MaterialLoader{target, textureCache};

    loader.load(scene, searchPath);
}

auto AssetLoader::importMeshes(aiScene const & scene, AssetRepository & target) const
    -> void
{
    auto const loader = MeshLoader{target};

    loader.load(scene);
}

auto AssetLoader::importModel(
    aiScene const & scene,
    AssetRepository & target,
    std::string name) const
    -> void
{
    auto const loader = ModelPartImporter{target};

    auto rootPart = loader.importRootPart(scene);

    target.models.emplace_back(std::move(rootPart), std::move(name));
}

} // namespace ape
