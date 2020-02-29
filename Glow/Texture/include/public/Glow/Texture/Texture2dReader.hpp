#pragma once

#include <Glow/Texture/TextureCube.hpp>
#include <Glow/Texture/Texture2d.hpp>
#include <Glow/Texture/TextureFiltering.hpp>
#include <Glow/Texture/TextureInternalFormat.hpp>
#include <Glow/Texture/TextureWrapping.hpp>

#include <Basix/FileSystem/FileFinder.hpp>

#include <filesystem>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

namespace glow
{

enum class TextureCubeFace;

class CouldNotFindImageFile : public std::logic_error
{

public:

    explicit CouldNotFindImageFile(std::filesystem::path const & path)
        : logic_error{"The image file '" + path.string() + "' was not found"}
    {
    }

};

class Texture2dReader
{

public:

    Texture2dReader() = default;

    explicit Texture2dReader(std::vector<std::filesystem::path> searchPaths);

    auto read(
        std::filesystem::path const & path,
        TextureInternalFormat internalFormat,
        TextureFiltering filtering,
        TextureWrapping wrapping = TextureWrapping::repeat,
        int const numOfMipmapLevels = 0,
        std::string_view label = "") const
        -> Texture2d;

    auto getSearchPaths() const
        -> std::vector<std::filesystem::path>;

private:

    auto resolveToPathOfExistingFile(std::filesystem::path const & path) const
        -> std::filesystem::path;

private:

    basix::FileFinder fileFinder;

};

} // namespace glow
