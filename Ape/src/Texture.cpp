#include <Ape/Texture.hpp>

#include <glad/glad.h>

#include <cassert>

namespace ape
{

namespace
{

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

Texture::Texture(std::string filepath)
    : Texture{readTextureDescriptor(filepath), filepath}
{
}
    
Texture::Texture(TextureDescriptor descriptor, std::string filepath)
    : id{makeOpenGLTextureObject(descriptor)}
    , filepath{std::move(filepath)}
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

auto Texture::getFilepath() const
    -> std::string
{
    return filepath;
}

auto Texture::getFormat() const
    -> unsigned int
{
    return format;
}

auto Texture::getSize() const
    -> Size<int>
{
    return size;
}

} // namespace ape
