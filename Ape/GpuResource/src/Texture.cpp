#include <Ape/GpuResource/Texture.hpp>

#include <Ape/GpuResource/TextureStorageType.hpp>

#include <glad/glad.h>

#include <cassert>

namespace ape
{

namespace
{

auto setTextureWrapping(TextureDescriptor const & descriptor)
    -> void
{
    auto const wrappingMode = convertToOpenGLTextureWrapping(descriptor.wrapping);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrappingMode);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrappingMode);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

auto setImmutableTextureImageData(TextureDescriptor const & descriptor)
    -> void
{
    auto const imageFormat = convertToOpenGLImageFormat(descriptor.image.format);

    auto const internalFormat = convertToOpenGLInternalFormat(descriptor.internalFormat);

    auto const pixelType = convertToOpenGLPixelType(descriptor.image.pixelType);

    glTexStorage2D(
        GL_TEXTURE_2D,
        1,
        internalFormat,
        descriptor.image.size.width,
        descriptor.image.size.height);

    if (descriptor.image.bytes != nullptr)
    {
        glTexSubImage2D(
            GL_TEXTURE_2D,
            0,
            0,
            0,
            descriptor.image.size.width,
            descriptor.image.size.height,
            imageFormat,
            pixelType,
            descriptor.image.bytes);
    }
}

auto setMutableTextureImageData(TextureDescriptor const & descriptor)
    -> void
{
    auto const imageFormat = convertToOpenGLImageFormat(descriptor.image.format);

    auto const internalFormat = convertToOpenGLInternalFormat(descriptor.internalFormat);

    auto const pixelType = convertToOpenGLPixelType(descriptor.image.pixelType);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        internalFormat,
        descriptor.image.size.width,
        descriptor.image.size.height,
        0,
        imageFormat,
        pixelType,
        descriptor.image.bytes);
}

auto setTextureImageData(TextureDescriptor const & descriptor)
    -> void
{
    if (descriptor.storageType == TextureStorageType::modifiable)
    {
        setMutableTextureImageData(descriptor);
    }
    else
    {
        assert(descriptor.storageType == TextureStorageType::immutable);

        setImmutableTextureImageData(descriptor);
    }
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

Texture::Texture(TextureDescriptor const & descriptor)
    : resource{makeOpenGLTextureObject(descriptor)}
    , size{descriptor.image.size}
    , internalFormat{descriptor.internalFormat}
    , imageFormat{descriptor.image.format}
    , pixelType{descriptor.image.pixelType}
    , wrapping{descriptor.wrapping}
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

auto Texture::getImageFormat() const
    -> TextureImageFormat
{
    return imageFormat;
}

auto Texture::getInternalFormat() const
    -> TextureInternalFormat
{
    return internalFormat;
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
    
    size = newSize;

    recreateMutableStorage();
}

auto Texture::recreateMutableStorage()
    -> void
{
    auto const openGLImageFormat = convertToOpenGLImageFormat(imageFormat);

    auto const openGLInternalFormat = convertToOpenGLInternalFormat(internalFormat);

    auto const openGLPixelType = convertToOpenGLPixelType(pixelType);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        openGLInternalFormat,
        size.width,
        size.height,
        0,
        openGLImageFormat,
        openGLPixelType,
        nullptr);
}

} // namespace ape
