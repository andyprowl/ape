#include <Glow/Texture/Texture.hpp>

#include <Glow/Texture/TextureStorageType.hpp>
#include <Glow/Texture/TextureSwizzleMask.hpp>

#include <Glow/GpuResource/ScopedBinder.hpp>

#include <Basix/Mathematics/Math.hpp>

#include <glad/glad.h>

#include <array>
#include <cassert>
#include <cmath>

namespace glow
{

namespace
{

auto setTextureFiltering(TextureDescriptor const & descriptor)
    -> void
{
    auto const magnification = convertToOpenGLTextureMagnificationFilter(
        descriptor.filtering.magnification);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magnification);

    auto const minification = convertToOpenGLTextureMinificationFilter(
        descriptor.filtering.minification);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minification);
}

auto setTextureWrapping(TextureDescriptor const & descriptor)
    -> void
{
    auto const wrapping = convertToOpenGLTextureWrapping(descriptor.wrapping);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapping);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapping);
}

auto determineNumOfMipmapLevels(TextureDescriptor const & descriptor)
    -> int
{
    if (descriptor.numOfMipmapLevels > 0)
    {
        return descriptor.numOfMipmapLevels;
    }

    auto const & baseSize = descriptor.image.size;

    return basix::log2(std::max(baseSize.width, baseSize.height));
}

auto setImmutableTextureImageData(TextureDescriptor const & descriptor)
    -> void
{
    auto const imageFormat = convertToOpenGLImageFormat(descriptor.image.format);

    auto const internalFormat = convertToOpenGLInternalFormat(descriptor.internalFormat);

    auto const pixelType = convertToOpenGLPixelType(descriptor.image.pixelType);

    auto const numOfMipmapLevels = determineNumOfMipmapLevels(descriptor);

    glTexStorage2D(
        GL_TEXTURE_2D,
        numOfMipmapLevels,
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
            descriptor.image.bytes.get());
    }
}

auto setMutableTextureImageData(TextureDescriptor const & descriptor)
    -> void
{
    auto const imageFormat = convertToOpenGLImageFormat(descriptor.image.format);

    auto const internalFormat = convertToOpenGLInternalFormat(descriptor.internalFormat);

    auto const pixelType = convertToOpenGLPixelType(descriptor.image.pixelType);

    auto const numOfMipmapLevels = determineNumOfMipmapLevels(descriptor);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, numOfMipmapLevels);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        internalFormat,
        descriptor.image.size.width,
        descriptor.image.size.height,
        0,
        imageFormat,
        pixelType,
        descriptor.image.bytes.get());
}

auto setTextureImageData(TextureDescriptor const & descriptor, bool const createMipmap)
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

    if (createMipmap)
    {
        glGenerateMipmap(GL_TEXTURE_2D);
    }
}

auto makeOpenGLTextureObject(TextureDescriptor const & descriptor, bool const createMipmap)
    -> GpuResource
{
    auto textureId = GpuResource::Id{};

    glGenTextures(1, &textureId);

    glBindTexture(GL_TEXTURE_2D, textureId);

    setTextureFiltering(descriptor);

    setTextureWrapping(descriptor);

    setTextureImageData(descriptor, createMipmap);

    glBindTexture(GL_TEXTURE_2D, 0);

    return GpuResource{textureId, [] (GpuResource::Id const id) { glDeleteTextures(1, &id); }};
}

} // unnamed namespace

Texture::Texture(TextureDescriptor const & descriptor)
    : Texture{descriptor, false, ""}
{
}

Texture::Texture(
    TextureDescriptor const & descriptor,
    bool const createMipmap,
    std::string_view const label)
    : resource{makeOpenGLTextureObject(descriptor, createMipmap)}
    , size{descriptor.image.size}
    , internalFormat{descriptor.internalFormat}
    , imageFormat{descriptor.image.format}
    , pixelType{descriptor.image.pixelType}
    , wrapping{descriptor.wrapping}
{
    setLabel(label);
}

auto Texture::getId() const
    -> GpuResource::Id
{
    return resource.get();
}

auto Texture::activate(int const unit) const
    -> void
{
    glActiveTexture(GL_TEXTURE0 + unit);

    bind();
}

auto Texture::bind() const
    -> void
{
    auto const id = resource.get();

    glBindTexture(GL_TEXTURE_2D, id);
}

auto Texture::unbind() const
    -> void
{
    glBindTexture(GL_TEXTURE_2D, 0);
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
    -> basix::Size<int>
{
    return size;
}

auto Texture::setSize(basix::Size<int> const & newSize)
    -> void
{
    if (newSize == size)
    {
        return;
    }
    
    size = newSize;

    recreateMutableStorage();
}

auto Texture::setSwizzleMask(TextureSwizzleMask const & mask)
    -> void
{
    auto const binder = glow::bind(*this);

    auto const swizzleMask = convertToOpenGLSwizzleMask(mask);

    glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask.data());

    assert(glGetError() == GL_NO_ERROR);
}

auto Texture::generateMipmap()
    -> void
{
    auto const binder = glow::bind(*this);

    glGenerateMipmap(GL_TEXTURE_2D);
}

auto Texture::setLabel(std::string_view const label)
    -> void
{
    glObjectLabel(GL_TEXTURE, getId(), static_cast<GLsizei>(label.size()), label.data());
}

auto Texture::recreateMutableStorage()
    -> void
{
    auto const binder = glow::bind(*this);

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

    assert(glGetError() == GL_NO_ERROR);
}

} // namespace glow
