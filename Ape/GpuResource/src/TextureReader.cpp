#include <Ape/GpuResource/TextureReader.hpp>

#include <Ape/GpuResource/CubeTextureFace.hpp>

#include <glad/glad.h>

#include <stb/stb_image.h>

#include <cassert>
#include <string>
#include <unordered_map>

namespace ape
{

namespace
{

auto const faceCodeMap = std::unordered_map<CubeTextureFace, std::string>{
    {CubeTextureFace::right, "right"},
    {CubeTextureFace::left, "left"},
    {CubeTextureFace::top, "top"},
    {CubeTextureFace::bottom, "bottom"},
    {CubeTextureFace::front, "front"},
    {CubeTextureFace::back, "back"}};

auto determineImageFormat(int const numOfChannels)
    -> TextureImageFormat
{
    switch (numOfChannels)
    {
        case 3:
        {
            return TextureImageFormat::rgb;
        }

        case 4:
        {
            return TextureImageFormat::rgba;
        }

        default:
        {
            assert(false);

            return TextureImageFormat::unknown;
        }
    }
}

auto chooseInternalFormat(TextureImageFormat const format)
    -> TextureInternalFormat
{
    switch (format)
    {

        case TextureImageFormat::rgb:
        {
            return TextureInternalFormat::rgb8;
        }

        case TextureImageFormat::rgba:
        {
            return TextureInternalFormat::rgba8;
        }

        default:
        {
            assert(false);

            return TextureInternalFormat::unknown;
        }
    }
}

auto makeTextureDescriptor(
    Size<int> const & size,
    TextureImageFormat const imageFormat,
    stbi_uc * const imageBytes)
    -> TextureDescriptor
{
    auto const bytes = reinterpret_cast<std::byte *>(imageBytes);

    auto const pixelType = PixelType::unsignedByte;

    auto const internalFormat = chooseInternalFormat(imageFormat);

    return {size, imageFormat, internalFormat, pixelType, bytes};
}

auto readTextureDescriptor(std::filesystem::path const & path, bool const flipVertically)
    -> TextureDescriptor
{
    stbi_set_flip_vertically_on_load(flipVertically);

    auto size = Size<int>{0, 0};

    auto numOfChannels = 0;

    auto const pathString = path.string();

    auto const bytes = stbi_load(pathString.c_str(), &size.width, &size.height, &numOfChannels, 0);

    if (bytes == nullptr)
    {
        throw CouldNotReadTexture{path};
    }

    auto format = determineImageFormat(numOfChannels);

    return makeTextureDescriptor(size, format, bytes);
}

} // unnamed namespace

class TextureReader::CubeTextureFacePathProvider
{

public:

    explicit CubeTextureFacePathProvider(std::filesystem::path const & facePath)
        : parentPath{facePath.parent_path()}
        , extension{facePath.extension().string()}
    {
    }

    auto getFacePath(CubeTextureFace const face) const
        -> std::filesystem::path
    {
        auto faceFileName = faceCodeMap.at(face) + extension;

        return (parentPath / std::move(faceFileName));
    }

private:

    std::filesystem::path parentPath;

    std::string extension;

};

TextureReader::TextureReader(std::vector<std::filesystem::path> searchPaths)
    : fileFinder{std::move(searchPaths)}
{
}

auto TextureReader::read(
    std::filesystem::path const & path,
    TextureStorageType const storageType) const
    -> Texture
{
    auto const absolutePath = resolveToPathOfExistingFile(path);

    auto const descriptor = readTextureDescriptor(absolutePath, true);

    auto const wrapping = TextureWrapping::repeat;

    auto texture = Texture{descriptor, wrapping, storageType};

    stbi_image_free(static_cast<void *>(descriptor.bytes));

    return texture;
}

auto TextureReader::readCubeTexture(
    std::filesystem::path const & facePath,
    TextureStorageType const storageType) const
    -> CubeTexture
{
    auto const facePathGenerator = CubeTextureFacePathProvider{facePath};

    auto descriptor = CubeTextureDescriptor{
        readCubeTextureFaceDescriptor(CubeTextureFace::right, facePathGenerator),
        readCubeTextureFaceDescriptor(CubeTextureFace::left, facePathGenerator),
        readCubeTextureFaceDescriptor(CubeTextureFace::top, facePathGenerator),
        readCubeTextureFaceDescriptor(CubeTextureFace::bottom, facePathGenerator),
        readCubeTextureFaceDescriptor(CubeTextureFace::front, facePathGenerator),
        readCubeTextureFaceDescriptor(CubeTextureFace::back, facePathGenerator)};

    auto const wrapping = TextureWrapping::clampToEdge;

    return {std::move(descriptor), wrapping, storageType};
}

auto TextureReader::getSearchPaths() const
    -> std::vector<std::filesystem::path>
{
    return fileFinder.getSearchPaths();
}

auto TextureReader::readCubeTextureFaceDescriptor(
    CubeTextureFace const face,
    CubeTextureFacePathProvider const & pathProvider) const
    -> TextureDescriptor
{
    auto const backFacePath = pathProvider.getFacePath(face);

    auto const backFaceAbsolutePath = resolveToPathOfExistingFile(backFacePath);

    return readTextureDescriptor(backFaceAbsolutePath, false);
}

auto TextureReader::resolveToPathOfExistingFile(std::filesystem::path const & path) const
    -> std::filesystem::path
{
    auto const existingFilePath = fileFinder.findExistingFile(path);

    if (!existingFilePath)
    {
        throw CouldNotFindTextureFile{path};
    }

    return *existingFilePath;
}

} // namespace ape
