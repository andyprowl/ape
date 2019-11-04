#include <Ape/GpuResource/CubeTexture.hpp>

#include <Ape/GpuResource/TextureStorageType.hpp>

#include <glad/glad.h>

namespace ape
{

namespace
{

auto setTextureWrapping(TextureWrapping const wrapping)
    -> void
{
    auto const wrappingMode = convertToOpenGLTextureWrapping(wrapping);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, wrappingMode);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, wrappingMode);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, wrappingMode);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

auto setTextureImageData(
    GLenum const target,
    TextureDescriptor const & descriptor,
    TextureStorageType const storageType)
    -> void
{
    auto const imageFormat = convertToOpenGLImageFormat(descriptor.imageFormat);

    auto const internalFormat = convertToOpenGLInternalFormat(descriptor.internalFormat);

    auto const pixelType = convertToOpenGLPixelType(descriptor.pixelType);

    auto const width = descriptor.size.width;

    auto const height = descriptor.size.height;

    auto const bytes = descriptor.bytes;

    if (storageType == TextureStorageType::modifiable)
    {
        glTexImage2D(target, 0, internalFormat, width, height, 0, imageFormat, pixelType, bytes);
    }
    else
    {
        glTexSubImage2D(target, 0, 0, 0, width, height, imageFormat, pixelType, bytes);
    }
}

auto createTextureStorage(CubeTextureDescriptor const & descriptor)
    -> void
{
    auto const width = descriptor.front.size.width;

    auto const height = descriptor.front.size.height;
    
    glTexStorage2D(GL_TEXTURE_CUBE_MAP, 1, GL_RGB8, width, height);
}

auto setTextureImageData(
    CubeTextureDescriptor const & descriptor,
    TextureStorageType const storageType)
    -> void
{
    setTextureImageData(GL_TEXTURE_CUBE_MAP_POSITIVE_X, descriptor.right, storageType);

    setTextureImageData(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, descriptor.left, storageType);

    setTextureImageData(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, descriptor.top, storageType);

    setTextureImageData(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, descriptor.bottom, storageType);

    setTextureImageData(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, descriptor.front, storageType);

    setTextureImageData(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, descriptor.back, storageType);
}

auto makeOpenGLTextureObject(
    CubeTextureDescriptor const & descriptor,
    TextureWrapping const wrapping,
    TextureStorageType const storageType)
    -> GpuResource
{
    auto textureId = GpuResource::Id{};

    glGenTextures(1, &textureId);

    glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);

    setTextureWrapping(wrapping);

    if (storageType == TextureStorageType::immutable)
    {
        createTextureStorage(descriptor);
    }

    setTextureImageData(descriptor, storageType);

    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

    return GpuResource{textureId, [] (GpuResource::Id const id) { glDeleteTextures(1, &id); }};
}

} // unnamed namespace

CubeTexture::CubeTexture(
    CubeTextureDescriptor const & descriptor,
    TextureWrapping const wrapping,
    TextureStorageType const storageType)
    : resource{makeOpenGLTextureObject(descriptor, wrapping, storageType)}
{
}

auto CubeTexture::getId() const
    -> GpuResource::Id
{
    return resource.get();
}

auto CubeTexture::bind(int const unit) const
    -> void
{
    glActiveTexture(GL_TEXTURE0 + unit);

    auto const id = resource.get();

    glBindTexture(GL_TEXTURE_CUBE_MAP, id);
}

} // namespace ape
