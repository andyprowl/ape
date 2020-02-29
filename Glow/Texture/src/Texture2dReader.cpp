#include <Glow/Texture/Texture2dReader.hpp>

#include <Glow/Texture/Image2d.hpp>
#include <Glow/Texture/TextureCubeFace.hpp>

#include <Stb/stb_image.h>

namespace glow
{

Texture2dReader::Texture2dReader(std::vector<std::filesystem::path> searchPaths)
    : fileFinder{std::move(searchPaths)}
{
}

auto Texture2dReader::read(
    std::filesystem::path const & path,
    TextureInternalFormat const internalFormat,
    TextureFiltering const filtering,
    TextureWrapping const wrapping,
    int const numOfMipmapLevels,
    std::string_view const label) const
    -> Texture2d
{
    auto const absolutePath = resolveToPathOfExistingFile(path);

    auto const image = readImageFromFile(absolutePath, true);

    auto const descriptor = Texture2dDescriptor{
        image.size,
        internalFormat,
        filtering,
        wrapping,
        numOfMipmapLevels};
    
    auto texture = Texture2d{descriptor, label};
    
    texture.setImage(image, true);

    return texture;
}

auto Texture2dReader::getSearchPaths() const
    -> std::vector<std::filesystem::path>
{
    return fileFinder.getSearchPaths();
}

auto Texture2dReader::resolveToPathOfExistingFile(std::filesystem::path const & path) const
    -> std::filesystem::path
{
    auto const existingFilePath = fileFinder.findExistingPath(path);

    if (not existingFilePath.has_value())
    {
        throw CouldNotFindImageFile{path};
    }

    return *existingFilePath;
}

} // namespace glow
