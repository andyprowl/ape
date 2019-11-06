#pragma once

#include <Ape/GpuResource/TextureReader.hpp>

#include <glm/vec4.hpp>

#include <filesystem>
#include <string>
#include <vector>

enum class ColorSpace;

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

    auto load(aiScene const & scene, std::filesystem::path const & directory) const
        -> void;

private:

    auto preventTextureRealloaction(aiScene const & scene) const
        -> void;

    auto computeNumOfTextures(aiScene const & scene) const
        -> int;

    auto importMaterial(aiMaterial const & material, std::filesystem::path const & directory) const
        -> void;

    auto getAmbientColor(aiMaterial const & material) const
        -> glm::vec4;

    auto getShininess(aiMaterial const & material) const
        -> float;

    auto importTextures(
        aiMaterial const & material,
        aiTextureType const type,
        std::filesystem::path const & directory) const
        -> std::vector<Texture const *>;

    auto getOrReadTexture(std::filesystem::path const & path, ColorSpace const colorSpace) const
        -> Texture const &;

    auto readAndStoreTexture(std::filesystem::path const & path, ColorSpace const colorSpace) const
        -> Texture const &;

private:

    AssetRepository * assets;

    TextureCache * textureCache;

    TextureReader textureReader;

};

} // namespace ape
