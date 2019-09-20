#pragma once

#include "Texture.hpp"

#include <stdexcept>
#include <string>
#include <vector>

class TextureFilenameNotUnique : public std::logic_error
{

public:

    explicit TextureFilenameNotUnique(std::string filename)
        : logic_error{"A texture was already loaded from file '" + std::move(filename) + "'"}
    {
    }

};

class TextureRepository
{

public:

    auto addTexture(Texture texture)
        -> Texture &;

    auto findTexture(std::string const & filename) const
        -> Texture const *;

private:

    std::vector<Texture> textures;

};
