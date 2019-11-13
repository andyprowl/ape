#include <Ape/Texture/TextureReader.hpp>

#include <Ape/Texture/CubeTextureFace.hpp>

#include <Stb/stb_image.h>

namespace ape
{

namespace
{

auto readTextureDescriptor(
    std::filesystem::path const & path,
    ColorSpace const colorSpace,
    TextureStorageType const storageType)
    -> TextureDescriptor
{
    auto const image = readImageFromFile(path, true);

    auto const internalFormat = determineInternalFormat(image.format, colorSpace);

    auto const wrapping = TextureWrapping::repeat;

    return {image, internalFormat, wrapping, storageType};
}

} // unnamed namespace

TextureReader::TextureReader(std::vector<std::filesystem::path> searchPaths)
    : fileFinder{std::move(searchPaths)}
{
}

auto TextureReader::read(
    std::filesystem::path const & path,
    TextureStorageType const storageType,
    ColorSpace const imageColorSpace) const
    -> Texture
{
    auto const absolutePath = resolveToPathOfExistingFile(path);

    auto const descriptor = readTextureDescriptor(absolutePath, imageColorSpace, storageType);
    
    auto texture = Texture{descriptor};

    stbi_image_free(reinterpret_cast<void *>(descriptor.image.bytes));

    return texture;
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

} // namespace ape
