#include <Glow/Texture/TextureCubeReader.hpp>

#include <Glow/Texture/TextureCubeFace.hpp>

#include <Stb/stb_image.h>

#include <cassert>
#include <string>
#include <unordered_map>

namespace glow
{

namespace
{

auto const faceCodeMap = std::unordered_map<TextureCubeFace, std::string>{
    {TextureCubeFace::right, "right"},
    {TextureCubeFace::left, "left"},
    {TextureCubeFace::top, "top"},
    {TextureCubeFace::bottom, "bottom"},
    {TextureCubeFace::front, "front"},
    {TextureCubeFace::back, "back"}};

class CubeTextureFacePathProvider
{

public:

    explicit CubeTextureFacePathProvider(std::filesystem::path const & facePath)
        : parentPath{facePath.parent_path()}
        , extension{facePath.extension().string()}
    {
    }

    auto getFacePath(TextureCubeFace const face) const
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
    -> TextureCubeImageSet
{
    if (std::filesystem::is_empty(folderPath))
    {
        throw EmptyImageFolder{folderPath};
    }

    auto const it = std::filesystem::directory_iterator{folderPath};

    auto const facePathProvider = CubeTextureFacePathProvider{it->path()};

    return {
        readImageFromFile(facePathProvider.getFacePath(TextureCubeFace::right), false),
        readImageFromFile(facePathProvider.getFacePath(TextureCubeFace::left), false),
        readImageFromFile(facePathProvider.getFacePath(TextureCubeFace::top), false),
        readImageFromFile(facePathProvider.getFacePath(TextureCubeFace::bottom), false),
        readImageFromFile(facePathProvider.getFacePath(TextureCubeFace::front), false),
        readImageFromFile(facePathProvider.getFacePath(TextureCubeFace::back), false)};
}

auto readTextureDescriptor(
    std::filesystem::path const & folder,
    ColorSpace const colorSpace,
    TextureFiltering const filtering,
    TextureWrapping const wrapping,
    int const numOfMipmapLevels)
    -> TextureCubeDescriptor
{
    auto imageSet = readImageSetFromFolder(folder);

    auto const internalFormat = determineInternalFormat(imageSet.front.format, colorSpace);

    return {std::move(imageSet), internalFormat, filtering, wrapping, numOfMipmapLevels};
}

} // unnamed namespace

TextureCubeReader::TextureCubeReader(std::vector<std::filesystem::path> searchPaths)
    : fileFinder{std::move(searchPaths)}
{
}

auto TextureCubeReader::read(
    std::filesystem::path const & folderPath,
    ColorSpace const imageColorSpace,
    TextureFiltering const filtering,
    TextureWrapping const wrapping,
    int const numOfMipmapLevels,
    std::string_view const label) const
    -> TextureCube
{
    auto const absolutePath = resolveToPathOfExistingFolder(folderPath);

    auto const descriptor = readTextureDescriptor(
        absolutePath,
        imageColorSpace,
        filtering,
        wrapping,
        numOfMipmapLevels);
    
    return TextureCube{descriptor, true, label};
}

auto TextureCubeReader::getSearchPaths() const
    -> std::vector<std::filesystem::path>
{
    return fileFinder.getSearchPaths();
}

auto TextureCubeReader::resolveToPathOfExistingFolder(std::filesystem::path const & path) const
    -> std::filesystem::path
{
    auto const existingFilePath = fileFinder.findExistingPath(path);

    if (not existingFilePath.has_value())
    {
        throw CouldNotFindImageFolder{path};
    }

    return *existingFilePath;
}

} // namespace glow
