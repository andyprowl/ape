#include <Glow/Texture/Texture2dReader.hpp>

#include <Glow/Texture/TextureCubeFace.hpp>

#include <Stb/stb_image.h>

namespace glow
{

namespace
{

auto readTextureDescriptor(
    std::filesystem::path const & path,
    ColorSpace const colorSpace,
    TextureFiltering const filtering,
    TextureWrapping const wrapping,
    int const numOfMipmapLevels)
    -> Texture2dDescriptor
{
    auto image = readImageFromFile(path, true);

    auto const internalFormat = determineInternalFormat(image.format, colorSpace);

    return {std::move(image), internalFormat, filtering, wrapping, numOfMipmapLevels};
}

} // unnamed namespace

Texture2dReader::Texture2dReader(std::vector<std::filesystem::path> searchPaths)
    : fileFinder{std::move(searchPaths)}
{
}

auto Texture2dReader::read(
    std::filesystem::path const & path,
    ColorSpace const imageColorSpace,
    TextureFiltering const filtering,
    TextureWrapping const wrapping,
    int const numOfMipmapLevels,
    std::string_view const label) const
    -> Texture2d
{
    auto const absolutePath = resolveToPathOfExistingFile(path);

    auto const descriptor = readTextureDescriptor(
        absolutePath,
        imageColorSpace,
        filtering,
        wrapping,
        numOfMipmapLevels);
    
    return Texture2d{descriptor, true, label};
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
