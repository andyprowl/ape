#pragma once

#include <filesystem>
#include <stdexcept>
#include <string>
#include <unordered_map>

namespace glow
{

class Texture;

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

    auto registerTexture(glow::Texture & texture, std::filesystem::path const & path)
        -> void;

    auto findTexture(std::filesystem::path const & path) const
        -> glow::Texture *;

private:

    std::unordered_map<std::string, glow::Texture *> cache;

};

} // namespace ape
