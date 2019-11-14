#include <Glow/Texture/CubeTextureReader.hpp>

#include <Glow/Texture/CubeTextureFace.hpp>

#include <Stb/stb_image.h>

#include <cassert>
#include <string>
#include <unordered_map>

namespace glow
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

class CubeTextureFacePathProvider
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

auto readImageSetFromFolder(std::filesystem::path const & folderPath)
    -> CubeTextureImageSet
{
    if (std::filesystem::is_empty(folderPath))
    {
        throw EmptyImageFolder{folderPath};
    }

    auto const it = std::filesystem::directory_iterator{folderPath};

    auto const facePathProvider = CubeTextureFacePathProvider{it->path()};

    return {
        readImageFromFile(facePathProvider.getFacePath(CubeTextureFace::right), false),
        readImageFromFile(facePathProvider.getFacePath(CubeTextureFace::left), false),
        readImageFromFile(facePathProvider.getFacePath(CubeTextureFace::top), false),
        readImageFromFile(facePathProvider.getFacePath(CubeTextureFace::bottom), false),
        readImageFromFile(facePathProvider.getFacePath(CubeTextureFace::front), false),
        readImageFromFile(facePathProvider.getFacePath(CubeTextureFace::back), false)};
}

auto freeImageSetBytes(CubeTextureImageSet const & imageSet)
    -> void
{
    stbi_image_free(reinterpret_cast<void *>(imageSet.front.bytes));

    stbi_image_free(reinterpret_cast<void *>(imageSet.back.bytes));

    stbi_image_free(reinterpret_cast<void *>(imageSet.left.bytes));

    stbi_image_free(reinterpret_cast<void *>(imageSet.right.bytes));

    stbi_image_free(reinterpret_cast<void *>(imageSet.top.bytes));

    stbi_image_free(reinterpret_cast<void *>(imageSet.bottom.bytes));
}

auto readTextureDescriptor(
    std::filesystem::path const & folder,
    ColorSpace const colorSpace)
    -> CubeTextureDescriptor
{
    auto const imageSet = readImageSetFromFolder(folder);

    auto const internalFormat = determineInternalFormat(imageSet.front.format, colorSpace);

    auto const wrapping = TextureWrapping::clampToEdge;

    return {imageSet, internalFormat, wrapping};
}

} // unnamed namespace

CubeTextureReader::CubeTextureReader(std::vector<std::filesystem::path> searchPaths)
    : fileFinder{std::move(searchPaths)}
{
}

auto CubeTextureReader::read(
    std::filesystem::path const & folderPath,
    ColorSpace const imageColorSpace) const
    -> CubeTexture
{
    auto const absolutePath = resolveToPathOfExistingFolder(folderPath);

    auto const descriptor = readTextureDescriptor(absolutePath, imageColorSpace);
    
    auto texture = CubeTexture{descriptor};

    freeImageSetBytes(descriptor.imageSet);

    return texture;
}

auto CubeTextureReader::getSearchPaths() const
    -> std::vector<std::filesystem::path>
{
    return fileFinder.getSearchPaths();
}

auto CubeTextureReader::resolveToPathOfExistingFolder(std::filesystem::path const & path) const
    -> std::filesystem::path
{
    auto const existingFilePath = fileFinder.findExistingPath(path);

    if (!existingFilePath)
    {
        throw CouldNotFindImageFolder{path};
    }

    return *existingFilePath;
}

} // namespace glow
