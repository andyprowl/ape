#pragma once

#include <filesystem>
#include <stdexcept>
#include <string>

struct aiScene;

namespace ape
{

class AssetRepository;

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

    auto load(std::filesystem::path path, std::string modelName) const
        -> AssetRepository;

private:

    auto load(aiScene const & scene, std::string modelName, std::filesystem::path source) const
        -> AssetRepository;

    auto load(
        aiScene const & scene,
        std::string modelName,
        std::filesystem::path source,
        AssetRepository & target) const
        -> void;

    auto importMaterials(
        aiScene const & scene,
        std::filesystem::path const & source,
        AssetRepository & target) const
        -> void;

    auto importMeshes(aiScene const & scene, AssetRepository & target) const
        -> void;

    auto importModel(
        aiScene const & scene,
        std::string name,
        std::filesystem::path source,
        AssetRepository & target) const
        -> void;

};

} // namespace ape
