#pragma once

#include "stb/stbimage.h"

#include <memory>
#include <stdexcept>
#include <string>

class CouldNotLoadTexture : public std::logic_error
{

public:

    CouldNotLoadTexture(std::string filename)
        : logic_error{"Failed to load texture '" + std::move(filename) + "'"}
    {
    }

};

class TextureDataDeleter
{

public:

    auto operator () (unsigned char * const data) const
        -> void
    {
        stbi_image_free(data);
    }

};

using TextureDataPtr = std::unique_ptr<unsigned char, TextureDataDeleter>;

class TextureData
{

public:

    TextureData()
        : width{0}
        , height{0}
        , numOfChannels{0}
        , data{nullptr}
    {
    }

public:

    int width;
    
    int height;

    int numOfChannels;

    TextureDataPtr data;

};

auto readTextureData(std::string const & filename)
    -> TextureData;

auto makeTexture(std::string const & filename, int format)
    -> int;
