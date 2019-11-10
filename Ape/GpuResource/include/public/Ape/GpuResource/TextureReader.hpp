#pragma once

#include <Ape/GpuResource/ColorSpace.hpp>
#include <Ape/GpuResource/CubeTexture.hpp>
#include <Ape/GpuResource/Texture.hpp>
#include <Ape/GpuResource/TextureStorageType.hpp>

#include <Foundational/FileSystem/FileFinder.hpp>

#include <filesystem>
#include <stdexcept>
#include <string>
#include <vector>

namespace ape
{

enum class CubeTextureFace;

class CouldNotFindImageFile : public std::logic_error
{

public:

    explicit CouldNotFindImageFile(std::filesystem::path const & path)
        : logic_error{"The image file '" + path.string() + "' was not found"}
    {
    }

};

class TextureReader
{

public:

    TextureReader() = default;

    explicit TextureReader(std::vector<std::filesystem::path> searchPaths);

    auto read(
        std::filesystem::path const & path,
        TextureStorageType storageType,
        ColorSpace imageColorSpace) const
        -> Texture;

    auto getSearchPaths() const
        -> std::vector<std::filesystem::path>;

private:

    auto resolveToPathOfExistingFile(std::filesystem::path const & path) const
        -> std::filesystem::path;

private:

    FileFinder fileFinder;

};

} // namespace ape
