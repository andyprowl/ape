#pragma once

#include <Ape/GpuResource/FileFinder.hpp>
#include <Ape/GpuResource/CubeTexture.hpp>
#include <Ape/GpuResource/Texture.hpp>

#include <filesystem>
#include <stdexcept>
#include <string>
#include <vector>

namespace ape
{

enum class CubeTextureFace;

class CouldNotFindTextureFile : public std::logic_error
{

public:

    explicit CouldNotFindTextureFile(std::filesystem::path const & path)
        : logic_error{"The texture file '" + path.string() + "' was not found"}
    {
    }

};

class CouldNotReadTexture : public std::logic_error
{

public:

    explicit CouldNotReadTexture(std::filesystem::path const & path)
        : logic_error{"Failed to read texture file '" + path.string() + "'"}
    {
    }

};

class TextureReader
{

public:

    TextureReader() = default;

    explicit TextureReader(std::vector<std::filesystem::path> searchPaths);

    auto read(std::filesystem::path const & path) const
        -> Texture;

    auto readCubeTexture(std::filesystem::path const & facePath) const
        -> CubeTexture;

    auto getSearchPaths() const
        -> std::vector<std::filesystem::path>;

private:

    class CubeTextureFacePathProvider;

private:

    auto readCubeTextureFaceDescriptor(
        CubeTextureFace face,
        CubeTextureFacePathProvider const & pathProvider) const
        -> TextureDescriptor;

    auto resolveToPathOfExistingFile(std::filesystem::path const & path) const
        -> std::filesystem::path;

private:

    FileFinder fileFinder;

};

} // namespace ape
