#include "MaterialLoader.hpp"

#include <assimp/scene.h>

MaterialLoader::MaterialLoader(AssetRepository & assets)
    : assets{&assets}
{
}

auto MaterialLoader::load(aiScene const & scene, std::string const & directory) const
    -> void
{
    auto materials = std::vector<Material>{};

    materials.reserve(scene.mNumMaterials);

    for (auto i = 0u; i < scene.mNumMaterials; ++i)
    {
        auto const descriptor = scene.mMaterials[i];

        importMaterial(*descriptor, directory);
    }
}

auto MaterialLoader::importMaterial(
    aiMaterial const & material,
    std::string const & directory) const
    -> void
{
    auto filename = aiString{};

    auto const ambientColor = getAmbientColor(material);

    auto const & diffuseMap = importTexture(material, aiTextureType_DIFFUSE, directory);

    auto const & specularMap = importTexture(material, aiTextureType_SPECULAR, directory);

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

auto MaterialLoader::importTexture(
    aiMaterial const & material,
    aiTextureType const type,
    std::string const & directory) const
    -> Texture const &
{
    auto numOfTextures = material.GetTextureCount(aiTextureType_DIFFUSE);

    // TODO: Will need to add support for multiple textures
    (void)numOfTextures;

    auto filename = aiString{};

    material.GetTexture(type, 0, &filename);

    auto const filePath = directory + '/' + filename.C_Str();

    return importTexture(filename.C_Str());
}

auto MaterialLoader::importTexture(std::string const & path) const
    -> Texture const &
{
    auto const existingTexture = findTexture(path);

    if (existingTexture != nullptr)
    {
        return *existingTexture;
    }

    auto newTexture = Texture{path};

    assets->textures.push_back(std::move(newTexture));

    return assets->textures.back();
}

auto MaterialLoader::findTexture(std::string const & filename) const
    -> Texture const *
{
    auto const it = std::find_if(
        std::cbegin(assets->textures),
        std::cend(assets->textures),
        [&filename] (Texture const & t)
    {
        return (t.getFilename() == filename);
    });

    if (it == std::cend(assets->textures))
    {
        return nullptr;
    }

    return &(*it);
}
