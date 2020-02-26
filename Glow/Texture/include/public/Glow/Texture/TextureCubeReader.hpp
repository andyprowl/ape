#pragma once

#include <Glow/Texture/ColorSpace.hpp>
#include <Glow/Texture/TextureCube.hpp>
#include <Glow/Texture/Texture2d.hpp>
#include <Glow/Texture/TextureFiltering.hpp>
#include <Glow/Texture/TextureWrapping.hpp>

#include <Basix/FileSystem/FileFinder.hpp>

#include <filesystem>
#include <stdexcept>
#include <string_view>
#include <vector>

namespace glow
{

enum class TextureCubeFace;

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

class TextureCubeReader
{

public:

    TextureCubeReader() = default;

    explicit TextureCubeReader(std::vector<std::filesystem::path> searchPaths);

    auto read(
        std::filesystem::path const & folderPath,
        ColorSpace imageColorSpace,
        TextureFiltering filtering,
        TextureWrapping wrapping = TextureWrapping::clampToEdge,
        int numOfMipmapLevels = 0,
        std::string_view label = "") const
        -> TextureCube;

    auto getSearchPaths() const
        -> std::vector<std::filesystem::path>;

private:

    auto resolveToPathOfExistingFolder(std::filesystem::path const & path) const
        -> std::filesystem::path;

private:

    basix::FileFinder fileFinder;

};

} // namespace glow
