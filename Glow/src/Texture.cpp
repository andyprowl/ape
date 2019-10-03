#include <Glow/Texture.hpp>

#include <glad/glad.h>

#include <cassert>
#include <unordered_map>

namespace ape
{

namespace
{

auto const textureFormatMap = std::unordered_map<TextureFormat, GLint>{
    {TextureFormat::redOnly, GL_RED},
    {TextureFormat::redGreenOnly, GL_RG},
    {TextureFormat::redGreenBlue, GL_RGB},
    {TextureFormat::redGreenBlueAlpha, GL_RGBA}};

auto convertToOpenGLFormat(TextureFormat const & format)
    -> GLint
{
    auto const it = textureFormatMap.find(format);

    if (it == std::cend(textureFormatMap))
    {
        assert(false);

        return 0;
    }

    return it->second;
}

auto convertFromOpenGLFormat(GLint const format)
    -> TextureFormat
{
    for (auto const & entry : textureFormatMap)
    {
        if (entry.second == format)
        {
            return entry.first;
        }
    }

    assert(false);

    return TextureFormat::unknown;
}

auto setDefaultTextureOptions()
    -> void
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

auto setTextureImageData(TextureDescriptor const & descriptor)
    -> void
{
    auto const format = convertToOpenGLFormat(descriptor.format);

    auto const width = descriptor.size.width;

    auto const height = descriptor.size.height;

    auto const bytes = descriptor.bytes;

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, bytes);
}

auto makeOpenGLTextureObject(TextureDescriptor const & descriptor)
    -> unsigned int
{
    auto textureId = unsigned int{};

    glGenTextures(1, &textureId);

    glBindTexture(GL_TEXTURE_2D, textureId);

    setDefaultTextureOptions();

    setTextureImageData(descriptor);

    glGenerateMipmap(GL_TEXTURE_2D);

    return textureId;
}

} // unnamed namespace

Texture::Texture(TextureDescriptor descriptor)
    : id{makeOpenGLTextureObject(descriptor)}
{
}

Texture::Texture(Texture && rhs) noexcept
    : id{rhs.id}
{
    rhs.id = 0;
}

auto Texture::operator = (Texture && rhs) noexcept
    -> Texture &
{
    destroy();

    id = rhs.id;

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
    -> TextureFormat
{
    glBindTexture(GL_TEXTURE_2D, id);

    auto const mipLevel = 0;

    auto format = 0;

    glGetTexLevelParameteriv(GL_TEXTURE_2D, mipLevel, GL_TEXTURE_INTERNAL_FORMAT, &format);

    return convertFromOpenGLFormat(format);
}

auto Texture::getSize() const
    -> Size<int>
{
    glBindTexture(GL_TEXTURE_2D, id);

    auto const mipLevel = 0;

    auto width = 0;
    
    auto height = 0;
    
    glGetTexLevelParameteriv(GL_TEXTURE_2D, mipLevel, GL_TEXTURE_WIDTH, &width);
    
    glGetTexLevelParameteriv(GL_TEXTURE_2D, mipLevel, GL_TEXTURE_HEIGHT, &height);

    return {width, height};
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
