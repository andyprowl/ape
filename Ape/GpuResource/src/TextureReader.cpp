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
    {CubeTextureFace::right, "rt"},
    {CubeTextureFace::left, "lf"},
    {CubeTextureFace::right, "up"},
    {CubeTextureFace::right, "dn"},
    {CubeTextureFace::right, "ft"},
    {CubeTextureFace::right, "bk"}};

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

    auto const pixelType = PixelType::unsignedByte;

    return {size, format, pixelType, bytes};
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

class TextureReader::CubeTextureFacePathProvider
{

public:

    explicit CubeTextureFacePathProvider(std::filesystem::path const & facePath)
        : parentPath{facePath.parent_path()}
        , extension{facePath.extension().string()}
        , fileNamePrefix{getFileNamePrefix(facePath)}
    {
    }

    auto getFacePath(CubeTextureFace const face) const
        -> std::filesystem::path
    {
        auto faceFileName = fileNamePrefix + faceCodeMap.at(face) + extension;

        return (parentPath / std::move(faceFileName));
    }

private:

    auto getFileNamePrefix(std::filesystem::path const & facePath) const
        -> std::string
    {
        auto const fileName = facePath.filename().string();

        return fileName.substr(0, fileName.length() - 2);
    }

private:

    std::filesystem::path parentPath;

    std::string extension;

    std::string fileNamePrefix;

};

TextureReader::TextureReader(std::vector<std::filesystem::path> searchPaths)
    : fileFinder{std::move(searchPaths)}
{
}

auto TextureReader::read(std::filesystem::path const & path) const
    -> Texture
{
    auto const absolutePath = resolveToPathOfExistingFile(path);

    auto const descriptor = readTextureDescriptor(absolutePath);

    auto const wrapping = TextureWrapping::repeat;

    auto texture = Texture{descriptor, wrapping};

    stbi_image_free(static_cast<void *>(descriptor.bytes));

    return texture;
}

auto TextureReader::readCubeTexture(std::filesystem::path const & facePath) const
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

    auto const wrapping = TextureWrapping::repeat;

    return {std::move(descriptor), wrapping};
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

    return readTextureDescriptor(backFaceAbsolutePath);
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
