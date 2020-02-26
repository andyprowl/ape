#pragma once

#include <filesystem>
#include <stdexcept>
#include <string>
#include <unordered_map>

namespace glow
{

class Texture2d;

} // namespace glow

namespace ape
{

class TexturePathNotUnique : public std::logic_error
{

public:

    explicit TexturePathNotUnique(std::filesystem::path const & path)
        : logic_error{"The texture file '" + path.string() + "' has already been registered"}
    {
    }

};

class TextureCache
{

public:

    auto registerTexture(glow::Texture2d & texture, std::filesystem::path const & path)
        -> void;

    auto findTexture(std::filesystem::path const & path) const
        -> glow::Texture2d *;

private:

    std::unordered_map<std::string, glow::Texture2d *> cache;

};

} // namespace ape
