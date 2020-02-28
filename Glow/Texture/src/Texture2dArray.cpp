#include <Glow/Texture/Texture2dArray.hpp>

#include <Glow/Texture/Image2d.hpp>

#include <Glow/GpuResource/ScopedBinder.hpp>

#include <Basix/Mathematics/Math.hpp>

#include <glad/glad.h>

namespace glow
{

namespace
{

auto setTextureFitering(GpuResource::Id const textureId, TextureFiltering const filtering)
    -> void
{
    auto const magnification = convertToOpenGLTextureMagnificationFilter(filtering.magnification);

    glTextureParameteri(textureId, GL_TEXTURE_MAG_FILTER, magnification);

    auto const minification = convertToOpenGLTextureMinificationFilter(filtering.minification);

    glTextureParameteri(textureId, GL_TEXTURE_MIN_FILTER, minification);
}

auto setTextureWrapping(GpuResource::Id const textureId, TextureWrapping const wrapping)
    -> void
{
    auto const wrappingMode = convertToOpenGLTextureWrapping(wrapping);

    glTextureParameteri(textureId, GL_TEXTURE_WRAP_S, wrappingMode);

    glTextureParameteri(textureId, GL_TEXTURE_WRAP_T, wrappingMode);
}

auto determineNumOfMipmapLevels(Texture2dDescriptor const & faceDescriptor)
    -> int
{
    if (faceDescriptor.numOfMipmapLevels > 0)
    {
        return faceDescriptor.numOfMipmapLevels;
    }

    return basix::log2(std::max(faceDescriptor.size.width, faceDescriptor.size.height));
}

auto createTextureStorage(
    GpuResource::Id const textureId,
    Texture2dDescriptor const & faceDescriptor,
    int const numOfLayers)
    -> void
{
    auto const & imageSize = faceDescriptor.size;
    
    auto const internalFormat = convertToOpenGLInternalFormat(faceDescriptor.internalFormat);

    auto const numOfMimaps = determineNumOfMipmapLevels(faceDescriptor);

    glTextureStorage3D(
        textureId,
        numOfMimaps,
        internalFormat,
        imageSize.width,
        imageSize.height,
        numOfLayers);
}

auto makeOpenGLTextureObject(Texture2dDescriptor const & descriptor, int const numOfLayers)
    -> GpuResource
{
    auto textureId = GpuResource::Id{};

    glCreateTextures(GL_TEXTURE_2D_ARRAY, 1, &textureId);

    createTextureStorage(textureId, descriptor, numOfLayers);

    setTextureFitering(textureId, descriptor.filtering);

    setTextureWrapping(textureId, descriptor.wrapping);

    return GpuResource{textureId, [] (GpuResource::Id const id) { glDeleteTextures(1, &id); }};
}

} // unnamed namespace

Texture2dArray::Texture2dArray(
    Texture2dDescriptor const & layerDescriptor,
    int const numOfLayers,
    std::string_view const label)
    : resource{makeOpenGLTextureObject(layerDescriptor, numOfLayers)}
    , layerDescriptor{layerDescriptor}
    , numOfLayers{numOfLayers}
{
    setLabel(label);
}

auto Texture2dArray::getId() const
    -> GpuResource::Id
{
    return resource.get();
}

auto Texture2dArray::bind() const
    -> void
{
    auto const id = resource.get();

    glBindTexture(GL_TEXTURE_2D_ARRAY, id);
}

auto Texture2dArray::unbind() const
    -> void
{
    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
}

auto Texture2dArray::getLayerDescriptor() const
    -> Texture2dDescriptor const &
{
    return layerDescriptor;
}

auto Texture2dArray::setLayerImage(int const layer, Image2d const & image)
    -> void
{
    auto const id = getId();

    auto const imageFormat = convertToOpenGLImageFormat(image.format);

    auto const pixelType = convertToOpenGLPixelType(image.pixelType);

    auto const bytes = image.bytes.get();

    glTextureSubImage3D(
        id,
        0,
        0,
        0,
        layer,
        image.size.width,
        image.size.height,
        1,
        imageFormat,
        pixelType,
        bytes);
}

auto Texture2dArray::generateMipmap()
    -> void
{
    auto const id = getId();

    glGenerateTextureMipmap(id);
}

auto Texture2dArray::setLabel(std::string_view const label)
    -> void
{
    auto const id = getId();

    glObjectLabel(GL_TEXTURE, id, static_cast<GLsizei>(label.size()), label.data());
}

} // namespace glow
