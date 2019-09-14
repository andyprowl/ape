#include "Texture.hpp"

#include "stb/stbimage.h"

#include <cassert>

namespace
{

auto determineFormat(int const numOfChannels)
    -> GLenum
{
    switch (numOfChannels)
    {
        case 1:
        {
            return GL_RED;
        }

        case 3:
        {
            return GL_RGB;
        }

        case 4:
        {
            return GL_RGBA;
        }

        default:
        {
            assert(false);

            return GL_RED;
        }
    }
}

auto makeOpenGLTextureObject(TextureDescriptor const & descriptor)
    -> int
{
    auto textureId = unsigned int{};

    glGenTextures(1, &textureId);

    glBindTexture(GL_TEXTURE_2D, textureId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        descriptor.format,
        descriptor.size.width,
        descriptor.size.height,
        0,
        descriptor.format,
        GL_UNSIGNED_BYTE,
        descriptor.bytes.get());

    glGenerateMipmap(GL_TEXTURE_2D);

    return textureId;
}

} // unnamed namespace

Texture::Texture(std::string filename)
    : Texture{readTextureDescriptor(filename), filename}
{
}
    
Texture::Texture(TextureDescriptor descriptor, std::string filename)
    : id{makeOpenGLTextureObject(descriptor)}
    , filename{std::move(filename)}
    , format{determineFormat(descriptor.numOfChannels)}
    , size{descriptor.size}
{
}

auto Texture::bind(int const unit) const
    -> void
{
    glActiveTexture(GL_TEXTURE0 + unit);

    glBindTexture(GL_TEXTURE_2D, id);
}

auto Texture::getFilename() const
    -> std::string
{
    return filename;
}

auto Texture::getFormat() const
    -> GLenum
{
    return format;
}

auto Texture::getSize() const
    -> Size<int>
{
    return size;
}

auto readTextureDescriptor(std::string const & filename)
    -> TextureDescriptor
{
    const auto filepath = std::string{textureFolder} + "/" + filename;

    stbi_set_flip_vertically_on_load(true);

    auto size = Size<int>{0, 0};

    auto numOfChannels = 0;

    auto const bytes = stbi_load(filepath.c_str(), &size.width, &size.height, &numOfChannels, 0);

    if (bytes == nullptr)
    {
        throw CouldNotLoadTexture{filename};
    }

    auto format = determineFormat(numOfChannels);

    return {size, numOfChannels, format, TextureBytesPtr{bytes}};
}
