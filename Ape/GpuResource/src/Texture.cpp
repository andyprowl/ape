#include <Ape/GpuResource/Texture.hpp>

#include <Ape/GpuResource/TextureStorageType.hpp>

#include <glad/glad.h>

#include <cassert>

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

auto setImmutableTextureImageData(TextureDescriptor const & descriptor)
    -> void
{
    auto const imageFormat = convertToOpenGLImageFormat(descriptor.imageFormat);

    auto const internalFormat = convertToOpenGLInternalFormat(descriptor.internalFormat);

    auto const pixelType = convertToOpenGLPixelType(descriptor.pixelType);

    glTexStorage2D(
        GL_TEXTURE_2D,
        1,
        internalFormat,
        descriptor.size.width,
        descriptor.size.height);

    if (descriptor.bytes != nullptr)
    {
        glTexSubImage2D(
            GL_TEXTURE_2D,
            0,
            0,
            0,
            descriptor.size.width,
            descriptor.size.height,
            imageFormat,
            pixelType,
            descriptor.bytes);
    }
}

auto setMutableTextureImageData(TextureDescriptor const & descriptor)
    -> void
{
    auto const imageFormat = convertToOpenGLImageFormat(descriptor.imageFormat);

    auto const internalFormat = convertToOpenGLInternalFormat(descriptor.internalFormat);

    auto const pixelType = convertToOpenGLPixelType(descriptor.pixelType);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        internalFormat,
        descriptor.size.width,
        descriptor.size.height,
        0,
        imageFormat,
        pixelType,
        descriptor.bytes);
}

auto setTextureImageData(
    TextureDescriptor const & descriptor,
    TextureStorageType const storageType)
    -> void
{
    if (storageType == TextureStorageType::modifiable)
    {
        setMutableTextureImageData(descriptor);
    }
    else
    {
        assert(storageType == TextureStorageType::immutable);

        setImmutableTextureImageData(descriptor);
    }
}

auto makeOpenGLTextureObject(
    TextureDescriptor const & descriptor,
    TextureWrapping const wrapping,
    TextureStorageType const storageType)
    -> GpuResource
{
    auto textureId = GpuResource::Id{};

    glGenTextures(1, &textureId);

    glBindTexture(GL_TEXTURE_2D, textureId);

    setTextureWrapping(wrapping);

    setTextureImageData(descriptor, storageType);

    glGenerateMipmap(GL_TEXTURE_2D);

    return GpuResource{textureId, [] (GpuResource::Id const id) { glDeleteTextures(1, &id); }};
}

} // unnamed namespace

Texture::Texture(
    TextureDescriptor const & descriptor,
    TextureWrapping const wrapping,
    TextureStorageType const storageType)
    : resource{makeOpenGLTextureObject(descriptor, wrapping, storageType)}
    , size{descriptor.size}
    , internalFormat{descriptor.internalFormat}
    , imageFormat{descriptor.imageFormat}
    , pixelType{descriptor.pixelType}
    , wrapping{wrapping}
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
