#include <AssetLoader/AssetLoader.hpp>

#include <AssetLoader/MaterialLoader.hpp>
#include <AssetLoader/MeshLoader.hpp>
#include <AssetLoader/ModelPartImporter.hpp>
#include <AssetLoader/TextureCache.hpp>

#include <Asset/AssetRepository.hpp>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <algorithm>

namespace ape
{

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

auto AssetLoader::load(std::string path, std::string modelName) const
    -> AssetRepository
{
    auto importer = Assimp::Importer{};

    const auto scene = importer.ReadFile(path, aiProcess_Triangulate); 

    if (scene == nullptr)
    {
        throw CouldNotLoadAssets(path, importer.GetErrorString());
    }

    return load(*scene, std::move(modelName), std::move(path));
}

auto AssetLoader::load(
    aiScene const & scene,
    std::string modelName,
    std::string source) const
    -> AssetRepository
{
    auto repository = AssetRepository{};

    load(scene, std::move(modelName), std::move(source), repository);

    return repository;
}

auto AssetLoader::load(
    aiScene const & scene,
    std::string modelName,
    std::string source,
    AssetRepository & target) const
    -> void
{
    importMaterials(scene, source, target);

    importMeshes(scene, target);

    importModel(scene, std::move(modelName), std::move(source), target);
}

auto AssetLoader::importMaterials(
    aiScene const & scene,
    std::string const & source,
    AssetRepository & target) const
    -> void
{
    auto textureCache = TextureCache{};

    auto const loader = MaterialLoader{target, textureCache};

    auto const directory = extractDirectory(source);

    loader.load(scene, directory);
}

auto AssetLoader::importMeshes(aiScene const & scene, AssetRepository & target) const
    -> void
{
    auto const loader = MeshLoader{target};

    loader.load(scene);
}

auto AssetLoader::importModel(
    aiScene const & scene,
    std::string name,
    std::string source,
    AssetRepository & target) const
    -> void
{
    auto const loader = ModelPartImporter{target};

    auto rootPart = loader.importRootPart(scene);

    target.models.emplace_back(std::move(rootPart), std::move(name), std::move(source));
}

} // namespace ape
