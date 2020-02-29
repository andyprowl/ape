#pragma once

#include <Glow/Texture/Texture2dReader.hpp>

#include <glm/vec4.hpp>

#include <filesystem>
#include <string>
#include <vector>

enum aiTextureType : int;

struct aiMaterial;
struct aiScene;

namespace glow
{

enum class ColorSpace;

class Texture2d;

} // namespace glow

namespace ape
{

class AssetRepository;
class TextureCache;

class MaterialLoader
{

public:

    MaterialLoader(
        AssetRepository & assets,
        TextureCache & textureCache,
        bool enableTextureCompression);

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
        -> std::vector<glow::Texture2d const *>;

    auto getOrReadTexture(
        std::filesystem::path const & path,
        glow::TextureInternalFormat internalFormat) const
        -> glow::Texture2d const &;

    auto readAndStoreTexture(
        std::filesystem::path const & path,
        glow::TextureInternalFormat internalFormat) const
        -> glow::Texture2d const &;

    auto getTextureInternalFormat(aiTextureType const type) const
        -> glow::TextureInternalFormat;

private:

    AssetRepository * assets;

    TextureCache * textureCache;

    glow::Texture2dReader textureReader;

    bool enableTextureCompression;

};

} // namespace ape
