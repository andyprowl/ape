#include <Ape/GpuResource/TextureReader.hpp>

#include <glad/glad.h>

#include <stb/stb_image.h>

#include <cassert>

namespace ape
{

namespace
{

auto determineFormat(int const numOfChannels)
    -> TextureFormat
{
    switch (numOfChannels)
    {
        case 1:
        {
            return TextureFormat::redOnly;
        }

        case 3:
        {
            return TextureFormat::redGreenBlue;
        }

        case 4:
        {
            return TextureFormat::redGreenBlueAlpha;
        }

        default:
        {
            assert(false);

            return TextureFormat::unknown;
        }
    }
}

auto makeTextureDescriptor(
    Size<int> const & size,
    TextureFormat const format,
    stbi_uc * const imageBytes)
    -> TextureDescriptor
{
    auto const bytes = reinterpret_cast<std::byte *>(imageBytes);

    auto const wrapping = TextureWrapping::repeat;

    auto const pixelType = PixelType::unsignedByte;

    return {size, format, pixelType, wrapping, bytes};
}

auto readTextureDescriptor(std::filesystem::path const & path)
    -> TextureDescriptor
{
    stbi_set_flip_vertically_on_load(true);

    auto size = Size<int>{0, 0};

    auto numOfChannels = 0;

    auto const pathString = path.string();

    auto const bytes = stbi_load(pathString.c_str(), &size.width, &size.height, &numOfChannels, 0);

    if (bytes == nullptr)
    {
        throw CouldNotReadTexture{path};
    }

    auto format = determineFormat(numOfChannels);

    return makeTextureDescriptor(size, format, bytes);
}

} // unnamed namespace

TextureReader::TextureReader(std::vector<std::filesystem::path> searchPaths)
    : fileFinder{std::move(searchPaths)}
{
}

auto TextureReader::read(std::filesystem::path const & path) const
    -> Texture
{
    auto const absolutePath = resolveToPathOfExistingFile(path);

    auto const descriptor = readTextureDescriptor(absolutePath);

    auto texture = Texture{descriptor};

    stbi_image_free(static_cast<void *>(descriptor.bytes));

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
    auto existingFilePath = fileFinder.findExistingFile(path);

    if (!existingFilePath)
    {
        throw CouldNotFindTextureFile{path};
    }

    return *existingFilePath;
}

} // namespace ape
