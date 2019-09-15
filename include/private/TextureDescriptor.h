#pragma once

#include "Size.hpp"

#include "glad/glad.h"
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

class TextureBytesDeleter
{

public:

    auto operator () (unsigned char * const data) const
        -> void
    {
        stbi_image_free(data);
    }

};

using TextureBytesPtr = std::unique_ptr<unsigned char, TextureBytesDeleter>;

class TextureDescriptor
{

public:

    TextureDescriptor(
        Size<int> const size,
        int const numOfChannels,
        GLenum format,
        TextureBytesPtr bytes)
        : size{size}
        , numOfChannels{numOfChannels}
        , format{format}
        , bytes{std::move(bytes)}
    {
    }

public:

    Size<int> size;

    int numOfChannels;

    GLenum format;

    TextureBytesPtr bytes;

};

auto readTextureDescriptor(std::string const & filename)
    -> TextureDescriptor;

auto determineFormat(int const numOfChannels)
    -> GLenum;
