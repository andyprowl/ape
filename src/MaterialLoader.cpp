#include "MaterialLoader.hpp"

#include <assimp/scene.h>

namespace
{

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

} // unnamed

MaterialLoader::MaterialLoader(AssetRepository & assets)
    : assets{&assets}
{
}

auto MaterialLoader::load(aiScene const & scene, std::string const & directory) const
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
    std::string const & directory) const
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
    std::string const & directory) const
    -> std::vector<Texture const *>
{
    auto numOfTextures = material.GetTextureCount(aiTextureType_DIFFUSE);

    auto textures = std::vector<Texture const *>{};

    textures.reserve(numOfTextures);

    for (auto i = 0u; i < numOfTextures; ++i)
    {
        auto filename = getTextureFilename(material, type, i);

        if (filename.empty())
        {
            continue;
        }

        auto const path = directory + '/' + std::move(filename);

        auto const & texture = importTexture(path);

        textures.push_back(&texture);
    }

    return textures;
}

auto MaterialLoader::importTexture(std::string const & path) const
    -> Texture const &
{
    auto const existingTexture = findTexture(path);

    if (existingTexture != nullptr)
    {
        return *existingTexture;
    }

    return assets->textures.emplace_back(std::move(path));
}

auto MaterialLoader::findTexture(std::string const & filepath) const
    -> Texture const *
{
    auto const it = std::find_if(
        std::cbegin(assets->textures),
        std::cend(assets->textures),
        [&filepath] (Texture const & t)
    {
        return (t.getFilepath() == filepath);
    });

    if (it == std::cend(assets->textures))
    {
        return nullptr;
    }

    return &(*it);
}
