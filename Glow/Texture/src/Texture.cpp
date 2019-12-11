#include <Glow/Texture/Texture.hpp>

#include <Glow/Texture/TextureStorageType.hpp>
#include <Glow/Texture/TextureSwizzleMask.hpp>

#include <Glow/GpuResource/ScopedBinder.hpp>

#include <glad/glad.h>

#include <array>
#include <cassert>

namespace glow
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
            descriptor.image.bytes.get());
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
        descriptor.image.bytes.get());
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
    : Texture{descriptor, ""}
{
}

Texture::Texture(TextureDescriptor const & descriptor, std::string_view const label)
    : resource{makeOpenGLTextureObject(descriptor)}
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
