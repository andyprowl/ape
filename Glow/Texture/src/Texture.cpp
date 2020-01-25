#include <Glow/Texture/Texture.hpp>

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

auto setTextureFiltering(GpuResource::Id const textureId, TextureDescriptor const & descriptor)
    -> void
{
    auto const magnification = convertToOpenGLTextureMagnificationFilter(
        descriptor.filtering.magnification);

    glTextureParameteri(textureId, GL_TEXTURE_MAG_FILTER, magnification);

    auto const minification = convertToOpenGLTextureMinificationFilter(
        descriptor.filtering.minification);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minification);
}

auto setTextureWrapping(GpuResource::Id const textureId, TextureDescriptor const & descriptor)
    -> void
{
    auto const wrapping = convertToOpenGLTextureWrapping(descriptor.wrapping);

    glTextureParameteri(textureId, GL_TEXTURE_WRAP_S, wrapping);

    glTextureParameteri(textureId, GL_TEXTURE_WRAP_T, wrapping);
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

auto setImmutableTextureImageData(
    GpuResource::Id const textureId,
    TextureDescriptor const & descriptor)
    -> void
{
    auto const imageFormat = convertToOpenGLImageFormat(descriptor.image.format);

    auto const internalFormat = convertToOpenGLInternalFormat(descriptor.internalFormat);

    auto const pixelType = convertToOpenGLPixelType(descriptor.image.pixelType);

    auto const numOfMipmapLevels = determineNumOfMipmapLevels(descriptor);

    glTextureStorage2D(
        textureId,
        numOfMipmapLevels,
        internalFormat,
        descriptor.image.size.width,
        descriptor.image.size.height);

    if (descriptor.image.bytes != nullptr)
    {
        glTextureSubImage2D(
            textureId,
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

auto setTextureImageData(
    GpuResource::Id const textureId,
    TextureDescriptor const & descriptor,
    bool const createMipmap)
    -> void
{
    setImmutableTextureImageData(textureId, descriptor);

    if (createMipmap)
    {
        glGenerateTextureMipmap(textureId);
    }
}

auto makeOpenGLTextureObject(TextureDescriptor const & descriptor, bool const createMipmap)
    -> GpuResource
{
    auto textureId = GpuResource::Id{};

    glCreateTextures(GL_TEXTURE_2D, 1, &textureId);

    setTextureFiltering(textureId, descriptor);

    setTextureWrapping(textureId, descriptor);

    setTextureImageData(textureId, descriptor, createMipmap);

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

auto Texture::setSwizzleMask(TextureSwizzleMask const & mask)
    -> void
{
    auto const swizzleMask = convertToOpenGLSwizzleMask(mask);

    glTextureParameteriv(getId(), GL_TEXTURE_SWIZZLE_RGBA, swizzleMask.data());

    assert(glGetError() == GL_NO_ERROR);
}

auto Texture::generateMipmap()
    -> void
{
    glGenerateTextureMipmap(getId());
}

auto Texture::setLabel(std::string_view const label)
    -> void
{
    glObjectLabel(GL_TEXTURE, getId(), static_cast<GLsizei>(label.size()), label.data());
}

} // namespace glow
