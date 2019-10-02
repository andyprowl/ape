#pragma once

#include <glm/vec4.hpp>

#include <string>
#include <vector>

enum aiTextureType : int;

struct aiMaterial;
struct aiScene;

namespace ape
{

class AssetRepository;
class Texture;
class TextureCache;

class MaterialLoader
{

public:

    MaterialLoader(AssetRepository & assets, TextureCache & textureCache);

    auto load(aiScene const & scene, std::string const & directory) const
        -> void;

private:

    auto preventTextureRealloaction(aiScene const & scene) const
        -> void;

    auto computeNumOfTextures(aiScene const & scene) const
        -> int;

    auto importMaterial(aiMaterial const & material, std::string const & directory) const
        -> void;

    auto getAmbientColor(aiMaterial const & material) const
        -> glm::vec4;

    auto getShininess(aiMaterial const & material) const
        -> float;

    auto importTextures(
        aiMaterial const & material,
        aiTextureType const type,
        std::string const & directory) const
        -> std::vector<Texture const *>;

    auto importTexture(std::string const & path) const
        -> Texture const &;

private:

    AssetRepository * assets;

    TextureCache * textureCache;

};

} // namespace ape
