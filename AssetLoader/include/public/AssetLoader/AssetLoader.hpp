#pragma once

#include <filesystem>
#include <stdexcept>
#include <string>

struct aiScene;

namespace ape
{

class AssetRepository;
class TextureCache;

class CouldNotLoadAssets : public std::logic_error
{

public:

    CouldNotLoadAssets(std::filesystem::path const & path, std::string error)
        : logic_error{
            "Could not load assets from file '" +
            path.string() +
            "': " +
            std::move(error)}
    {
    }

};

class AssetLoader
{

public:

    auto load(std::filesystem::path const & path, std::string modelName) const
        -> AssetRepository;

    auto load(
        std::filesystem::path const & path,
        TextureCache & textureCache,
        std::string modelName) const
        -> AssetRepository;

private:

    auto load(
        aiScene const & scene,
        std::filesystem::path const & searchPath,
        TextureCache & textureCache,
        std::string modelName) const
        -> AssetRepository;

    auto load(
        aiScene const & scene,
        std::filesystem::path const & searchPath,
        TextureCache & textureCache,
        AssetRepository & target,
        std::string modelName) const
        -> void;

    auto importMaterials(
        aiScene const & scene,
        std::filesystem::path const & searchPath,
        TextureCache & textureCache,
        AssetRepository & target) const
        -> void;

    auto importMeshes(aiScene const & scene, AssetRepository & target) const
        -> void;

    auto importModel(aiScene const & scene, AssetRepository & target, std::string name) const
        -> void;

};

} // namespace ape
