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

class CouldNotFindImageFolder : public std::logic_error
{

public:

    explicit CouldNotFindImageFolder(std::filesystem::path const & path)
        : logic_error{"The image folder '" + path.string() + "' was not found"}
    {
    }

};

class EmptyImageFolder : public std::logic_error
{

public:

    explicit EmptyImageFolder(std::filesystem::path const & path)
        : logic_error{"The image folder '" + path.string() + "' is empty"}
    {
    }

};

class CubeTextureReader
{

public:

    CubeTextureReader() = default;

    explicit CubeTextureReader(std::vector<std::filesystem::path> searchPaths);

    auto read(std::filesystem::path const & folderPath, ColorSpace imageColorSpace) const
        -> CubeTexture;

    auto getSearchPaths() const
        -> std::vector<std::filesystem::path>;

private:

    auto resolveToPathOfExistingFolder(std::filesystem::path const & path) const
        -> std::filesystem::path;

private:

    FileFinder fileFinder;

};

} // namespace ape
