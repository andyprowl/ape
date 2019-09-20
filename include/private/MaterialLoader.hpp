#pragma once

#include "AssetRepository.hpp"

#include <glm/vec4.hpp>

#include <string>

enum aiTextureType : int;

struct aiMaterial;
struct aiScene;

class MaterialLoader
{

public:

    explicit MaterialLoader(AssetRepository & assets);

    auto load(aiScene const & scene, std::string const & directory) const
        -> void;

private:

    auto importMaterial(aiMaterial const & material, std::string const & directory) const
        -> void;

    auto getAmbientColor(aiMaterial const & material) const
        -> glm::vec4;

    auto getShininess(aiMaterial const & material) const
        -> float;

    auto importTexture(
        aiMaterial const & material,
        aiTextureType const type,
        std::string const & directory) const
        -> Texture const &;

    auto importTexture(std::string const & path) const
        -> Texture const &;

    auto findTexture(std::string const & filename) const
        -> Texture const *;

private:

    AssetRepository * assets;

};
