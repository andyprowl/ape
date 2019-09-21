#pragma once

#include <stdexcept>
#include <string>

class AssetRepository;

struct aiScene;

class CouldNotLoadAssets : public std::logic_error
{

public:

    CouldNotLoadAssets(std::string path, std::string error)
        : logic_error{
            "Could not load assets from file '" +
            std::move(path) +
            "': " +
            std::move(error)}
    {
    }

};

class AssetLoader
{

public:

    auto load(std::string path, std::string modelName) const
        -> AssetRepository;

private:

    auto load(aiScene const & scene, std::string modelName, std::string source) const
        -> AssetRepository;

    auto load(
        aiScene const & scene,
        std::string modelName,
        std::string source,
        AssetRepository & target) const
        -> void;

    auto importMaterials(
        aiScene const & scene,
        std::string const & source,
        AssetRepository & target) const
        -> void;

    auto importMeshes(aiScene const & scene, AssetRepository & target) const
        -> void;

    auto importModel(
        aiScene const & scene,
        std::string name,
        std::string source,
        AssetRepository & target) const
        -> void;

};
