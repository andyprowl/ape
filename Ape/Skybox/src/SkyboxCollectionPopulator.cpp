#include <Ape/Skybox/SkyboxCollectionPopulator.hpp>

#include <Ape/Skybox/SkyboxCollection.hpp>

#include <Glow/Texture/TextureReader.hpp>

#include <filesystem>

namespace ape
{

namespace
{

const auto effectShaderSuffix = ".Fragment.glsl";

auto isCubeFaceBackTextureFile(std::filesystem::directory_entry const & entry)
    -> bool
{
    if (!entry.is_regular_file())
    {
        return false;
    }

    return (entry.path().filename().stem() == "back");
}

} // unnamed namespace

SkyboxCollectionPopulator::SkyboxCollectionPopulator(SkyboxCollection & collection)
    : collection{&collection}
{
}

auto SkyboxCollectionPopulator::addAllSkyboxesInFolder(
    std::filesystem::path const & skyboxFolder) const
    -> void
{
    for (auto const & entry : std::filesystem::recursive_directory_iterator{skyboxFolder})
    {
        if (!isCubeFaceBackTextureFile(entry))
        {
            continue;
        }

        auto const folder = entry.path().parent_path();

        auto skybox = textureReader.read(folder, glow::ColorSpace::perceptual);

        collection->addSkybox(std::move(skybox));
    }
}

} // namespace ape
