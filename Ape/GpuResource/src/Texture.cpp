#include <Ape/GpuResource/Texture.hpp>

#include <glad/glad.h>

namespace ape
{

namespace
{

auto setTextureWrapping(TextureWrapping const wrapping)
    -> void
{
    auto const wrappingMode = convertToOpenGLTextureWrapping(wrapping);

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

auto makeOpenGLTextureObject(
    TextureDescriptor const & descriptor,
    TextureWrapping const wrapping)
    -> GpuResource
{
    auto textureId = GpuResource::Id{};

    glGenTextures(1, &textureId);

    glBindTexture(GL_TEXTURE_2D, textureId);

    setTextureWrapping(wrapping);

    setTextureImageData(descriptor);

    glGenerateMipmap(GL_TEXTURE_2D);

    return GpuResource{textureId, [] (GpuResource::Id const id) { glDeleteTextures(1, &id); }};
}

} // unnamed namespace

Texture::Texture(TextureDescriptor descriptor, TextureWrapping const wrapping)
    : resource{makeOpenGLTextureObject(descriptor, wrapping)}
    , size{descriptor.size}
    , format{descriptor.format}
    , pixelType{descriptor.pixelType}
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
    return format;
}

auto Texture::getSize() const
    -> Size<int>
{
    return size;
}

auto Texture::setSize(Size<int> const & newSize)
    -> void
{
    if (newSize == size)
    {
        return;
    }

    auto const openGLFormat = convertToOpenGLFormat(format);

    auto const openGLPixelType = convertToOpenGLPixelType(pixelType);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        openGLFormat,
        newSize.width,
        newSize.height,
        0,
        openGLFormat,
        openGLPixelType,
        nullptr);
}

} // namespace ape
