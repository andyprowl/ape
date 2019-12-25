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
    TextureStorageType const storageType)
    -> TextureDescriptor
{
    auto image = readImageFromFile(path, true);

    auto const internalFormat = determineInternalFormat(image.format, colorSpace);

    auto const wrapping = TextureWrapping::repeat;

    return {std::move(image), internalFormat, wrapping, storageType};
}

} // unnamed namespace

TextureReader::TextureReader(std::vector<std::filesystem::path> searchPaths)
    : fileFinder{std::move(searchPaths)}
{
}

auto TextureReader::read(
    std::filesystem::path const & path,
    TextureStorageType const storageType,
    ColorSpace const imageColorSpace,
    std::string_view const label) const
    -> Texture
{
    auto const absolutePath = resolveToPathOfExistingFile(path);

    auto const descriptor = readTextureDescriptor(absolutePath, imageColorSpace, storageType);
    
    return Texture{descriptor, label};
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
