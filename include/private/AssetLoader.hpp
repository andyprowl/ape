#pragma once

#include "TextureRepository.hpp"

#include <string>

class AssetRepository;

struct aiScene;

class AssetLoader
{

public:

    auto load(aiScene const & scene, std::string const & directory) const
        -> AssetRepository;

    auto load(aiScene const & scene, std::string const & directory, AssetRepository & target) const
        -> void;

private:

    auto importMaterials(
        aiScene const & scene,
        std::string const & directory,
        AssetRepository & target) const
        -> void;

    auto importMeshes(aiScene const & scene, AssetRepository & target) const
        -> void;

    auto importModel(aiScene const & scene, AssetRepository & target) const
        -> void;

};
