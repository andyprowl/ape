#include <Glow/Texture/Texture2d.hpp>

#include <Glow/Texture/Image2d.hpp>
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

auto setTextureFiltering(GpuResource::Id const textureId, Texture2dDescriptor const & descriptor)
    -> void
{
    auto const magnification = convertToOpenGLTextureMagnificationFilter(
        descriptor.filtering.magnification);

    glTextureParameteri(textureId, GL_TEXTURE_MAG_FILTER, magnification);

    auto const minification = convertToOpenGLTextureMinificationFilter(
        descriptor.filtering.minification);

    glTextureParameteri(textureId, GL_TEXTURE_MIN_FILTER, minification);
}

auto setTextureWrapping(GpuResource::Id const textureId, Texture2dDescriptor const & descriptor)
    -> void
{
    auto const wrapping = convertToOpenGLTextureWrapping(descriptor.wrapping);

    glTextureParameteri(textureId, GL_TEXTURE_WRAP_S, wrapping);

    glTextureParameteri(textureId, GL_TEXTURE_WRAP_T, wrapping);
}

auto determineNumOfMipmapLevels(Texture2dDescriptor const & descriptor)
    -> int
{
    if (descriptor.numOfMipmapLevels > 0)
    {
        return descriptor.numOfMipmapLevels;
    }

    auto const & baseSize = descriptor.size;

    return basix::log2(std::max(baseSize.width, baseSize.height));
}

auto createTextureStorage(GpuResource::Id const textureId, Texture2dDescriptor const & descriptor)
    -> void
{
    auto const internalFormat = convertToOpenGLInternalFormat(descriptor.internalFormat);

    auto const numOfMipmapLevels = determineNumOfMipmapLevels(descriptor);

    glTextureStorage2D(
        textureId,
        numOfMipmapLevels,
        internalFormat,
        descriptor.size.width,
        descriptor.size.height);
}

auto makeOpenGLTextureObject(Texture2dDescriptor const & descriptor)
    -> GpuResource
{
    auto textureId = GpuResource::Id{};

    glCreateTextures(GL_TEXTURE_2D, 1, &textureId);

    setTextureFiltering(textureId, descriptor);

    setTextureWrapping(textureId, descriptor);

    createTextureStorage(textureId, descriptor);

    return GpuResource{textureId, [] (GpuResource::Id const id) { glDeleteTextures(1, &id); }};
}

} // unnamed namespace

Texture2d::Texture2d(Texture2dDescriptor const & descriptor)
    : Texture2d{descriptor, ""}
{
}

Texture2d::Texture2d(Texture2dDescriptor const & descriptor, std::string_view const label)
    : resource{makeOpenGLTextureObject(descriptor)}
    , descriptor{descriptor}
{
    setLabel(label);
}

auto Texture2d::getId() const
    -> GpuResource::Id
{
    return resource.get();
}

auto Texture2d::bind() const
    -> void
{
    auto const id = resource.get();

    glBindTexture(GL_TEXTURE_2D, id);
}

auto Texture2d::unbind() const
    -> void
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

auto Texture2d::setSwizzleMask(TextureSwizzleMask const & mask)
    -> void
{
    auto const swizzleMask = convertToOpenGLSwizzleMask(mask);

    glTextureParameteriv(getId(), GL_TEXTURE_SWIZZLE_RGBA, swizzleMask.data());

    assert(glGetError() == GL_NO_ERROR);
}

auto Texture2d::getDescriptor() const
    -> Texture2dDescriptor const &
{
    return descriptor;
}

auto Texture2d::setImage(Image2d const & image, bool const createMipmap)
    -> void
{
    auto const id = getId();

    auto const & size = image.size;

    auto const imageFormat = convertToOpenGLImageFormat(image.format);

    auto const pixelType = convertToOpenGLPixelType(image.pixelType);

    auto const bytes = image.bytes.get();

    glTextureSubImage2D(id, 0, 0, 0, size.width, size.height, imageFormat, pixelType, bytes);

    if (createMipmap)
    {
        generateMipmap();
    }
}

auto Texture2d::generateMipmap()
    -> void
{
    auto const id = getId();

    glGenerateTextureMipmap(id);
}

auto Texture2d::setLabel(std::string_view const label)
    -> void
{
    glObjectLabel(GL_TEXTURE, getId(), static_cast<GLsizei>(label.size()), label.data());
}

} // namespace glow
