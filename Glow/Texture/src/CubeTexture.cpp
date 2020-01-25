#include <Glow/Texture/CubeTexture.hpp>

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

    glTextureParameteri(textureId, GL_TEXTURE_WRAP_R, wrappingMode);

    glTextureParameteri(textureId, GL_TEXTURE_WRAP_S, wrappingMode);

    glTextureParameteri(textureId, GL_TEXTURE_WRAP_T, wrappingMode);
}

auto determineNumOfMipmapLevels(CubeTextureDescriptor const & descriptor)
    -> int
{
    if (descriptor.numOfMipmapLevels > 0)
    {
        return descriptor.numOfMipmapLevels;
    }

    // Assuming face images all have the same size, we pick the front one.
    auto const & baseSize = descriptor.imageSet.front.size;

    return basix::log2(std::max(baseSize.width, baseSize.height));
}

auto setTextureImageData(
    GpuResource::Id const textureId,
    GLenum const target,
    TextureImage const & image)
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

    auto const & bytes = image.bytes;

    auto const faceIndex = target - GL_TEXTURE_CUBE_MAP_POSITIVE_X;

    glTextureSubImage3D(
        textureId,
        0,
        0,
        0,
        faceIndex,
        width,
        height,
        1,
        imageFormat,
        pixelType,
        bytes.get());
}

auto createTextureStorage(GpuResource::Id const textureId, CubeTextureDescriptor const & descriptor)
    -> void
{
    auto const & imageSize = descriptor.imageSet.right.size;
    
    auto const internalFormat = convertToOpenGLInternalFormat(descriptor.internalFormat);

    auto const numOfMimaps = determineNumOfMipmapLevels(descriptor);

    glTextureStorage2D(
        textureId,
        numOfMimaps,
        internalFormat,
        imageSize.width,
        imageSize.height);
}

auto setTextureImageData(
    GpuResource::Id const textureId,
    CubeTextureImageSet const & imageSet,
    bool const createMipmap)
    -> void
{
    setTextureImageData(textureId, GL_TEXTURE_CUBE_MAP_POSITIVE_X, imageSet.right);

    setTextureImageData(textureId, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, imageSet.left);

    setTextureImageData(textureId, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, imageSet.top);

    setTextureImageData(textureId, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, imageSet.bottom);

    setTextureImageData(textureId, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, imageSet.front);

    setTextureImageData(textureId, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, imageSet.back);

    if (createMipmap)
    {
        glGenerateTextureMipmap(textureId);
    }
}

auto makeOpenGLTextureObject(CubeTextureDescriptor const & descriptor, bool const createMipmap)
    -> GpuResource
{
    auto textureId = GpuResource::Id{};

    glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &textureId);

    createTextureStorage(textureId, descriptor);

    setTextureFitering(textureId, descriptor.filtering);

    setTextureWrapping(textureId, descriptor.wrapping);

    setTextureImageData(textureId, descriptor.imageSet, createMipmap);

    return GpuResource{textureId, [] (GpuResource::Id const id) { glDeleteTextures(1, &id); }};
}

} // unnamed namespace

CubeTexture::CubeTexture(CubeTextureDescriptor const & descriptor)
    : CubeTexture{descriptor, false, ""}
{
}

CubeTexture::CubeTexture(
    CubeTextureDescriptor const & descriptor,
    bool const createMipmap,
    std::string_view const label)
    : resource{makeOpenGLTextureObject(descriptor, createMipmap)}
{
    setLabel(label);
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

auto CubeTexture::generateMipmap()
    -> void
{
    auto const binder = glow::bind(*this);

    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
}

auto CubeTexture::setLabel(std::string_view const label)
    -> void
{
    glObjectLabel(GL_TEXTURE, getId(), static_cast<GLsizei>(label.size()), label.data());
}

} // namespace glow
