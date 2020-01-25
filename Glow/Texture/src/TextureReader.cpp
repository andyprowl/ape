#include <Glow/Texture/TextureReader.hpp>

#include <Glow/Texture/CubeTextureFace.hpp>

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
    -> TextureDescriptor
{
    auto image = readImageFromFile(path, true);

    auto const internalFormat = determineInternalFormat(image.format, colorSpace);

    return {std::move(image), internalFormat, filtering, wrapping, numOfMipmapLevels};
}

} // unnamed namespace

TextureReader::TextureReader(std::vector<std::filesystem::path> searchPaths)
    : fileFinder{std::move(searchPaths)}
{
}

auto TextureReader::read(
    std::filesystem::path const & path,
    ColorSpace const imageColorSpace,
    TextureFiltering const filtering,
    TextureWrapping const wrapping,
    int const numOfMipmapLevels,
    std::string_view const label) const
    -> Texture
{
    auto const absolutePath = resolveToPathOfExistingFile(path);

    auto const descriptor = readTextureDescriptor(
        absolutePath,
        imageColorSpace,
        filtering,
        wrapping,
        numOfMipmapLevels);
    
    return Texture{descriptor, true, label};
}

auto TextureReader::getSearchPaths() const
    -> std::vector<std::filesystem::path>
{
    return fileFinder.getSearchPaths();
}

auto TextureReader::resolveToPathOfExistingFile(std::filesystem::path const & path) const
    -> std::filesystem::path
{
    auto const existingFilePath = fileFinder.findExistingPath(path);

    if (!existingFilePath)
    {
        throw CouldNotFindImageFile{path};
    }

    return *existingFilePath;
}

} // namespace glow
