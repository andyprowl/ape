#pragma once

#include "Size.h"

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

class Texture
{

public:

    explicit Texture(std::string filename);
    
    Texture(TextureDescriptor descriptor, std::string filename);

    auto bind(int unit) const
        -> void;

    auto getFilename() const
        -> std::string;

    auto getFormat() const
        -> GLenum;

    auto getSize() const
        -> Size<int>;

private:

    int id;

    std::string filename;

    GLenum format;

    Size<int> size;

};

auto readTextureDescriptor(std::string const & filename)
    -> TextureDescriptor;
