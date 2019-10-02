#pragma once

#include <Mathematics/Size.hpp>

#include <memory>
#include <stdexcept>
#include <string>

namespace ape
{

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
        -> void;

};

using TextureBytesPtr = std::unique_ptr<unsigned char, TextureBytesDeleter>;

class TextureDescriptor
{

public:

    TextureDescriptor(
        Size<int> const size,
        int const numOfChannels,
        unsigned int format,
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

    unsigned int format;

    TextureBytesPtr bytes;

};

auto readTextureDescriptor(std::string const & filepath)
    -> TextureDescriptor;

auto determineFormat(int const numOfChannels)
    -> unsigned int;

} // namespace ape
