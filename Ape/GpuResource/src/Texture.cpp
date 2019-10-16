#include <Ape/GpuResource/Texture.hpp>

#include <glad/glad.h>

#include <cassert>
#include <unordered_map>

namespace ape
{

namespace
{

auto const textureFormatMap = std::unordered_map<TextureFormat, GLenum>{
    {TextureFormat::depthOnly, GL_DEPTH_COMPONENT},
    {TextureFormat::depthAndStencil, GL_DEPTH_STENCIL},
    {TextureFormat::redOnly, GL_RED},
    {TextureFormat::redGreenOnly, GL_RG},
    {TextureFormat::redGreenBlue, GL_RGB},
    {TextureFormat::redGreenBlueAlpha, GL_RGBA}};

auto const pixelTypeMap = std::unordered_map<PixelType, GLenum>{
    {PixelType::unsignedByte, GL_UNSIGNED_BYTE},
    {PixelType::floatingPoint, GL_FLOAT}};

auto const textureWrapping = std::unordered_map<TextureWrapping, GLint>{
    {TextureWrapping::repeat, GL_REPEAT},
    {TextureWrapping::clampToEdge, GL_CLAMP_TO_EDGE}};

auto convertToOpenGLFormat(TextureFormat const format)
    -> GLenum
{
    auto const it = textureFormatMap.find(format);

    if (it == std::cend(textureFormatMap))
    {
        assert(false);

        return 0;
    }

    return it->second;
}

auto convertFromOpenGLFormat(GLenum const format)
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

auto convertToOpenGLPixelType(PixelType const type)
    -> GLenum
{
    auto const it = pixelTypeMap.find(type);

    if (it == std::cend(pixelTypeMap))
    {
        assert(false);

        return 0;
    }

    return it->second;
}

auto convertFromOpenGLPixelType(GLenum const type)
    -> PixelType
{
    for (auto const & entry : pixelTypeMap)
    {
        if (entry.second == type)
        {
            return entry.first;
        }
    }

    assert(false);

    return PixelType::unknown;
}

auto convertToOpenGLTextureWrapping(TextureWrapping const wrapping)
    -> GLint
{
    auto const it = textureWrapping.find(wrapping);

    if (it == std::cend(textureWrapping))
    {
        assert(false);

        return 0;
    }

    return it->second;
}

auto setTextureWrapping(TextureDescriptor const & descriptor)
    -> void
{
    auto const wrappingMode = convertToOpenGLTextureWrapping(descriptor.wrapping);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrappingMode);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrappingMode);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

auto setTextureImageData(TextureDescriptor const & descriptor)
    -> void
{
    auto const format = convertToOpenGLFormat(descriptor.format);

    auto const pixelType = convertToOpenGLPixelType(descriptor.pixelType);

    auto const width = descriptor.size.width;

    auto const height = descriptor.size.height;

    auto const bytes = descriptor.bytes;

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, pixelType, bytes);
}

auto makeOpenGLTextureObject(TextureDescriptor const & descriptor)
    -> GpuResource
{
    auto textureId = GpuResource::Id{};

    glGenTextures(1, &textureId);

    glBindTexture(GL_TEXTURE_2D, textureId);

    setTextureWrapping(descriptor);

    setTextureImageData(descriptor);

    glGenerateMipmap(GL_TEXTURE_2D);

    return GpuResource{textureId, [] (GpuResource::Id const id) { glDeleteTextures(1, &id); }};
}

} // unnamed namespace

Texture::Texture(TextureDescriptor descriptor)
    : resource{makeOpenGLTextureObject(descriptor)}
{
}

auto Texture::getId() const
    -> GpuResource::Id
{
    return resource.get();
}

auto Texture::bind(int const unit) const
    -> void
{
    glActiveTexture(GL_TEXTURE0 + unit);

    auto const id = resource.get();

    glBindTexture(GL_TEXTURE_2D, id);
}

auto Texture::getFormat() const
    -> TextureFormat
{
    auto const id = resource.get();

    glBindTexture(GL_TEXTURE_2D, id);

    auto const mipLevel = 0;

    auto format = 0;

    glGetTexLevelParameteriv(GL_TEXTURE_2D, mipLevel, GL_TEXTURE_INTERNAL_FORMAT, &format);

    return convertFromOpenGLFormat(format);
}

auto Texture::getSize() const
    -> Size<int>
{
    auto const id = resource.get();
    
    glBindTexture(GL_TEXTURE_2D, id);

    auto const mipLevel = 0;

    auto width = 0;
    
    auto height = 0;
    
    glGetTexLevelParameteriv(GL_TEXTURE_2D, mipLevel, GL_TEXTURE_WIDTH, &width);
    
    glGetTexLevelParameteriv(GL_TEXTURE_2D, mipLevel, GL_TEXTURE_HEIGHT, &height);

    return {width, height};
}

} // namespace ape
