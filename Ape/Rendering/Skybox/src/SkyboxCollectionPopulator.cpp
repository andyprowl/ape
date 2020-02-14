#include <Ape/Rendering/Skybox/SkyboxCollectionPopulator.hpp>

#include <Ape/Rendering/Skybox/SkyboxCollection.hpp>

#include <Glow/Texture/TextureReader.hpp>

#include <filesystem>

namespace ape
{

namespace
{

auto const effectShaderSuffix = ".Fragment.glsl";

auto isCubeFaceBackTextureFile(std::filesystem::directory_entry const & entry)
    -> bool
{
    if (not entry.is_regular_file())
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
        if (not isCubeFaceBackTextureFile(entry))
        {
            continue;
        }

        auto const folder = entry.path().parent_path();

        // For the skybox we do not want additional mipmap levels to be generated.
        auto const numOfMipmapLevels = 1;

        auto skybox = textureReader.read(
            folder,
            glow::ColorSpace::perceptual,
            glow::TextureFiltering{
                glow::TextureMagnificationFilter::linear,
                glow::TextureMinificationFilter::linear},
            glow::TextureWrapping::clampToEdge,
            numOfMipmapLevels);

        collection->addSkybox(std::move(skybox));
    }
}

} // namespace ape
