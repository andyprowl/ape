#include <Ape/GpuResource/CubeTexture.hpp>

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

auto setTextureImageData(GLenum const target, TextureDescriptor const & descriptor)
    -> void
{
    auto const format = convertToOpenGLFormat(descriptor.format);

    auto const pixelType = convertToOpenGLPixelType(descriptor.pixelType);

    auto const width = descriptor.size.width;

    auto const height = descriptor.size.height;

    auto const bytes = descriptor.bytes;

    glTexImage2D(target, 0, format, width, height, 0, format, pixelType, bytes);
}

auto setTextureImageData(CubeTextureDescriptor const & descriptor)
    -> void
{
    setTextureImageData(GL_TEXTURE_CUBE_MAP_POSITIVE_X, descriptor.right);

    setTextureImageData(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, descriptor.left);

    setTextureImageData(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, descriptor.top);

    setTextureImageData(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, descriptor.bottom);

    setTextureImageData(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, descriptor.front);

    setTextureImageData(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, descriptor.back);
}

auto makeOpenGLTextureObject(
    CubeTextureDescriptor const & descriptor,
    TextureWrapping const wrapping)
    -> GpuResource
{
    auto textureId = GpuResource::Id{};

    glGenTextures(1, &textureId);

    glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);

    setTextureWrapping(wrapping);

    setTextureImageData(descriptor);

    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

    return GpuResource{textureId, [] (GpuResource::Id const id) { glDeleteTextures(1, &id); }};
}

} // unnamed namespace

CubeTexture::CubeTexture(CubeTextureDescriptor descriptor, TextureWrapping const wrapping)
    : resource{makeOpenGLTextureObject(descriptor, wrapping)}
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
