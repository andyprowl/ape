#include <Ape/Rendering/SkyboxCollectionReader.hpp>

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

    return (entry.path().filename().stem() == "bk");
}

} // unnamed namespace

SkyboxCollectionReader::SkyboxCollectionReader()
    : skyboxFolder{resourceFolder "/skyboxes"}
{
}

auto SkyboxCollectionReader::read() const
    -> SkyboxCollection
{
    auto skyboxes = std::vector<CubeTexture>{};

    auto textureReader = TextureReader{};

    for (auto const & entry : std::filesystem::recursive_directory_iterator(skyboxFolder))
    {
        if (isCubeFaceBackTextureFile(entry))
        {
            auto cubeTexture = textureReader.readCubeTexture(entry.path());

            skyboxes.push_back(std::move(cubeTexture));
        }
    }

    return SkyboxCollection{std::move(skyboxes)};
}

} // namespace ape
