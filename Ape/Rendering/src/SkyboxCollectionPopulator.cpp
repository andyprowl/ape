#include <Ape/Rendering/SkyboxCollectionPopulator.hpp>

#include <Ape/Rendering/SkyboxCollection.hpp>

#include <Ape/GpuResource/TextureReader.hpp>

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
        if (isCubeFaceBackTextureFile(entry))
        {
            auto const storageType = TextureStorageType::immutable;

            auto cubeTexture = textureReader.readCubeTexture(entry.path(), storageType);

            collection->addSkybox(std::move(cubeTexture));
        }
    }
}

} // namespace ape
