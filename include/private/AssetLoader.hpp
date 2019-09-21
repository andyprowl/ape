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

    auto load(std::string const & path) const
        -> AssetRepository;

private:

    auto load(std::string const & source, aiScene const & scene) const
        -> AssetRepository;

    auto load(std::string const & source, aiScene const & scene, AssetRepository & target) const
        -> void;

    auto importMaterials(
        std::string const & source,
        aiScene const & scene,
        AssetRepository & target) const
        -> void;

    auto importMeshes(aiScene const & scene, AssetRepository & target) const
        -> void;

    auto importModel(
        std::string const & source,
        aiScene const & scene,
        AssetRepository & target) const
        -> void;

};
