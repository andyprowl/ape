#include <Ape/AssetLoader/MaterialLoader.hpp>

#include <Ape/AssetLoader/AssetRepository.hpp>
#include <Ape/AssetLoader/TextureCache.hpp>

#include <assimp/scene.h>

namespace ape
{

namespace
{

template<typename T>
auto makeVectorWithCapacity(std::size_t const capacity)
{
    auto v = std::vector<T>{};

    v.reserve(capacity);

    return v;
}

auto toString(aiString const & s)
    -> std::string
{
    return s.C_Str();
}

auto getTextureFilename(aiMaterial const & m, aiTextureType const type, int const index)
    -> std::string
{
    auto filename = aiString{};

    m.GetTexture(type, index, &filename);

    return toString(filename);
}

auto getTextureColorSpace(aiTextureType const type)
    -> ColorSpace
{
    return (type == aiTextureType_DIFFUSE) ? ColorSpace::perceptual : ColorSpace::linear;
}

} // unnamed

MaterialLoader::MaterialLoader(AssetRepository & assets, TextureCache & textureCache)
    : assets{&assets}
    , textureCache{&textureCache}
{
}

auto MaterialLoader::load(aiScene const & scene, std::filesystem::path const & directory) const
    -> void
{
    auto materials = std::vector<Material>{};

    materials.reserve(scene.mNumMaterials);

    preventTextureRealloaction(scene);

    for (auto i = 0u; i < scene.mNumMaterials; ++i)
    {
        auto const descriptor = scene.mMaterials[i];

        importMaterial(*descriptor, directory);
    }
}

auto MaterialLoader::preventTextureRealloaction(aiScene const & scene) const
    -> void
{
    auto numOfTextures = computeNumOfTextures(scene);

    assets->textures.reserve(static_cast<int>(numOfTextures));
}

auto MaterialLoader::computeNumOfTextures(aiScene const & scene) const
    -> int
{
    auto numOfTextures = 0;

    for (auto i = 0u; i < scene.mNumMaterials; ++i)
    {
        numOfTextures += scene.mMaterials[i]->GetTextureCount(aiTextureType_DIFFUSE);

        numOfTextures += scene.mMaterials[i]->GetTextureCount(aiTextureType_SPECULAR);
    }

    return numOfTextures;
}

auto MaterialLoader::importMaterial(
    aiMaterial const & material,
    std::filesystem::path const & directory) const
    -> void
{
    auto const diffuseMaps = importTextures(material, aiTextureType_DIFFUSE, directory);

    auto const specularMaps = importTextures(material, aiTextureType_SPECULAR, directory);

    auto const diffuseMap = diffuseMaps.empty() ? nullptr : diffuseMaps[0];

    auto const specularMap = specularMaps.empty() ? nullptr : specularMaps[0];

    auto const ambientColor = getAmbientColor(material);

    auto const shininess = getShininess(material);

    assets->materials.emplace_back(ambientColor, diffuseMap, specularMap, shininess);
}

auto MaterialLoader::getAmbientColor(aiMaterial const & material) const
    -> glm::vec4
{
    auto ambientColor = aiColor4D{};

    if (aiGetMaterialColor(&material, AI_MATKEY_COLOR_AMBIENT, &ambientColor) != AI_SUCCESS)
    {
        return {0.0f, 0.0f, 0.0f, 1.0f};
    }

    return {ambientColor.r, ambientColor.g, ambientColor.b, ambientColor.a};
}

auto MaterialLoader::getShininess(aiMaterial const & material) const
    -> float
{
    auto shininess = 0.0f;

    if (aiGetMaterialFloat(&material, AI_MATKEY_SHININESS, &shininess) != AI_SUCCESS)
    {
        shininess = 32.f;
    }

    return shininess;
}

auto MaterialLoader::importTextures(
    aiMaterial const & material,
    aiTextureType const type,
    std::filesystem::path const & directory) const
    -> std::vector<Texture const *>
{
    auto numOfTextures = material.GetTextureCount(type);

    auto textures = makeVectorWithCapacity<Texture const *>(numOfTextures);

    auto const colorSpace = getTextureColorSpace(type);

    for (auto i = 0u; i < numOfTextures; ++i)
    {
        auto filename = getTextureFilename(material, type, i);

        if (filename.empty())
        {
            continue;
        }

        auto const path = directory / std::move(filename);

        auto const & texture = getOrReadTexture(path, colorSpace);

        textures.push_back(&texture);
    }

    return textures;
}

auto MaterialLoader::getOrReadTexture(
    std::filesystem::path const & path,
    ColorSpace const colorSpace) const
    -> Texture const &
{
    auto const existingTexture = textureCache->findTexture(path);

    if (existingTexture != nullptr)
    {
        return *existingTexture;
    }

    return readAndStoreTexture(path, colorSpace);
}

auto MaterialLoader::readAndStoreTexture(
    std::filesystem::path const & path,
    ColorSpace const colorSpace) const
    -> Texture const &
{
    const auto storageType = TextureStorageType::immutable;

    auto readTexture = textureReader.read(path, storageType, colorSpace);

    auto & storedTexture = assets->textures.emplace_back(std::move(readTexture));

    textureCache->registerTexture(storedTexture, path);

    return storedTexture;
}

} // namespace ape
