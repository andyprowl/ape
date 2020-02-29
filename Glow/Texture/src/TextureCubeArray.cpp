#include <Glow/Texture/TextureCubeArray.hpp>

#include <Glow/Texture/TextureCubeImageSet.hpp>

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

    auto const numOfLayerFaces = numOfLayers * 6;

    glTextureStorage3D(
        textureId,
        numOfMimaps,
        internalFormat,
        imageSize.width,
        imageSize.height,
        numOfLayerFaces);
}

auto makeOpenGLTextureObject(Texture2dDescriptor const & descriptor, int const numOfLayers)
    -> GpuResource
{
    auto textureId = GpuResource::Id{};

    glCreateTextures(GL_TEXTURE_CUBE_MAP_ARRAY, 1, &textureId);

    createTextureStorage(textureId, descriptor, numOfLayers);

    setTextureFitering(textureId, descriptor.filtering);

    setTextureWrapping(textureId, descriptor.wrapping);

    return GpuResource{textureId, [] (GpuResource::Id const id) { glDeleteTextures(1, &id); }};
}

auto setTextureImageData(
    GpuResource::Id const textureId,
    int const layer,
    GLenum const target,
    Image2d const & image)
    -> void
{
    if (image.bytes == nullptr)
    {
        return;
    }

    auto const imageFormat = convertToOpenGLImageFormat(image.format);

    auto const pixelType = convertToOpenGLPixelType(image.pixelType);

    auto const bytes = image.bytes.get();

    auto const faceIndex = target - GL_TEXTURE_CUBE_MAP_POSITIVE_X;

    auto const layerFaceIndex = layer * 6 + faceIndex;

    glTextureSubImage3D(
        textureId,
        0,
        0,
        0,
        layerFaceIndex,
        image.size.width,
        image.size.height,
        1,
        imageFormat,
        pixelType,
        bytes);
}

} // unnamed namespace

TextureCubeArray::TextureCubeArray(
    Texture2dDescriptor const & layerFaceDescriptor,
    int const numOfLayers,
    std::string_view const label)
    : resource{makeOpenGLTextureObject(layerFaceDescriptor, numOfLayers)}
    , layerFaceDescriptor{layerFaceDescriptor}
    , numOfLayers{numOfLayers}
{
    setLabel(label);
}

auto TextureCubeArray::getId() const
    -> GpuResource::Id
{
    return resource.get();
}

auto TextureCubeArray::bind() const
    -> void
{
    auto const id = resource.get();

    glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, id);
}

auto TextureCubeArray::unbind() const
    -> void
{
    glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, 0);
}

auto TextureCubeArray::getLayerFaceDescriptor() const
    -> Texture2dDescriptor const &
{
    return layerFaceDescriptor;
}

auto TextureCubeArray::setLayerImages(int const layer, TextureCubeImageSet const & images)
    -> void
{
    auto const id = getId();

    setTextureImageData(id, layer, GL_TEXTURE_CUBE_MAP_POSITIVE_X, images.right);

    setTextureImageData(id, layer,GL_TEXTURE_CUBE_MAP_NEGATIVE_X, images.left);

    setTextureImageData(id, layer,GL_TEXTURE_CUBE_MAP_POSITIVE_Y, images.top);

    setTextureImageData(id, layer,GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, images.bottom);

    setTextureImageData(id, layer,GL_TEXTURE_CUBE_MAP_POSITIVE_Z, images.front);

    setTextureImageData(id, layer,GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, images.back);
}

auto TextureCubeArray::generateMipmap()
    -> void
{
    auto const id = getId();

    glGenerateTextureMipmap(id);
}

auto TextureCubeArray::setLabel(std::string_view const label)
    -> void
{
    auto const id = getId();

    glObjectLabel(GL_TEXTURE, id, static_cast<GLsizei>(label.size()), label.data());
}

} // namespace glow
