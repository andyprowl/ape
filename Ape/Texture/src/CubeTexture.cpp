#include <Ape/Texture/CubeTexture.hpp>

#include <Ape/Texture/TextureStorageType.hpp>

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

auto setTextureImageData(GLenum const target, TextureImage const & image)
    -> void
{
    if (image.bytes == nullptr)
    {
        return;
    }

    auto const imageFormat = convertToOpenGLImageFormat(image.format);

    auto const pixelType = convertToOpenGLPixelType(image.pixelType);

    auto const width = image.size.width;

    auto const height = image.size.height;

    auto const bytes = image.bytes;

    glTexSubImage2D(target, 0, 0, 0, width, height, imageFormat, pixelType, bytes);
}

auto createTextureStorage(CubeTextureDescriptor const & descriptor)
    -> void
{
    auto const & imageSize = descriptor.imageSet.right.size;
    
    auto const internalFormat = convertToOpenGLInternalFormat(descriptor.internalFormat);

    glTexStorage2D(GL_TEXTURE_CUBE_MAP, 1, internalFormat, imageSize.width, imageSize.height);
}

auto setTextureImageData(CubeTextureImageSet const & imageSet)
    -> void
{
    setTextureImageData(GL_TEXTURE_CUBE_MAP_POSITIVE_X, imageSet.right);

    setTextureImageData(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, imageSet.left);

    setTextureImageData(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, imageSet.top);

    setTextureImageData(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, imageSet.bottom);

    setTextureImageData(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, imageSet.front);

    setTextureImageData(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, imageSet.back);
}

auto makeOpenGLTextureObject(CubeTextureDescriptor const & descriptor)
    -> GpuResource
{
    auto textureId = GpuResource::Id{};

    glGenTextures(1, &textureId);

    glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);

    createTextureStorage(descriptor);

    setTextureImageData(descriptor.imageSet);

    setTextureWrapping(descriptor.wrapping);

    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

    return GpuResource{textureId, [] (GpuResource::Id const id) { glDeleteTextures(1, &id); }};
}

} // unnamed namespace

CubeTexture::CubeTexture(CubeTextureDescriptor const & descriptor)
    : resource{makeOpenGLTextureObject(descriptor)}
{
}

auto CubeTexture::getId() const
    -> GpuResource::Id
{
    return resource.get();
}

auto CubeTexture::activate(int const unit) const
    -> void
{
    glActiveTexture(GL_TEXTURE0 + unit);

    bind();
}

auto CubeTexture::bind() const
    -> void
{
    auto const id = resource.get();

    glBindTexture(GL_TEXTURE_CUBE_MAP, id);
}

auto CubeTexture::unbind() const
    -> void
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

} // namespace ape
