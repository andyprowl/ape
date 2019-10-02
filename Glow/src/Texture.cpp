#include <Glow/Texture.hpp>

#include <glad/glad.h>

#include <cassert>

namespace ape
{

namespace
{

auto makeOpenGLTextureObject(TextureDescriptor const & descriptor)
    -> unsigned int
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

Texture::Texture(TextureDescriptor descriptor)
    : id{makeOpenGLTextureObject(descriptor)}
    , format{determineFormat(descriptor.numOfChannels)}
    , size{descriptor.size}
{
}

Texture::Texture(Texture && rhs) noexcept
    : id{rhs.id}
    , format{rhs.format}
    , size{rhs.size}
{
    rhs.id = 0;
}

auto Texture::operator = (Texture && rhs) noexcept
    -> Texture &
{
    destroy();

    id = rhs.id;
    
    format = rhs.format;
    
    size = rhs.size;

    rhs.id = 0;

    return *this;
}

Texture::~Texture()
{
    destroy();
}

auto Texture::bind(int const unit) const
    -> void
{
    glActiveTexture(GL_TEXTURE0 + unit);

    glBindTexture(GL_TEXTURE_2D, id);
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

auto Texture::destroy()
    -> void
{
    if (id != 0)
    {
        glDeleteTextures(1, &id);
    }
}

} // namespace ape
