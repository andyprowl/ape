#include <Glow/Texture/TextureCube.hpp>

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

    glTextureParameteri(textureId, GL_TEXTURE_WRAP_R, wrappingMode);

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

auto setTextureImageData(
    GpuResource::Id const textureId,
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

    glTextureSubImage3D(
        textureId,
        0,
        0,
        0,
        faceIndex,
        image.size.width,
        image.size.height,
        1,
        imageFormat,
        pixelType,
        bytes);
}

auto createTextureStorage(
    GpuResource::Id const textureId,
    Texture2dDescriptor const & faceDescriptor)
    -> void
{
    auto const & imageSize = faceDescriptor.size;
    
    auto const internalFormat = convertToOpenGLInternalFormat(faceDescriptor.internalFormat);

    auto const numOfMimaps = determineNumOfMipmapLevels(faceDescriptor);

    glTextureStorage2D(
        textureId,
        numOfMimaps,
        internalFormat,
        imageSize.width,
        imageSize.height);
}

auto makeOpenGLTextureObject(Texture2dDescriptor const & descriptor)
    -> GpuResource
{
    auto textureId = GpuResource::Id{};

    glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &textureId);

    createTextureStorage(textureId, descriptor);

    setTextureFitering(textureId, descriptor.filtering);

    setTextureWrapping(textureId, descriptor.wrapping);

    return GpuResource{textureId, [] (GpuResource::Id const id) { glDeleteTextures(1, &id); }};
}

} // unnamed namespace

TextureCube::TextureCube(Texture2dDescriptor const & faceDescriptor)
    : TextureCube{faceDescriptor, ""}
{
}

TextureCube::TextureCube(
    Texture2dDescriptor const & faceDescriptor,
    std::string_view const label)
    : resource{makeOpenGLTextureObject(faceDescriptor)}
    , faceDescriptor{faceDescriptor}
{
    setLabel(label);
}

auto TextureCube::getId() const
    -> GpuResource::Id
{
    return resource.get();
}

auto TextureCube::bind() const
    -> void
{
    auto const id = resource.get();

    glBindTexture(GL_TEXTURE_CUBE_MAP, id);
}

auto TextureCube::unbind() const
    -> void
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

auto TextureCube::getFaceDescriptor() const
    -> Texture2dDescriptor const &
{
    return faceDescriptor;
}

auto TextureCube::setFaceImages(TextureCubeImageSet const & images, bool const createMipmap)
    -> void
{
    auto const id = getId();

    setTextureImageData(id, GL_TEXTURE_CUBE_MAP_POSITIVE_X, images.right);

    setTextureImageData(id, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, images.left);

    setTextureImageData(id, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, images.top);

    setTextureImageData(id, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, images.bottom);

    setTextureImageData(id, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, images.front);

    setTextureImageData(id, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, images.back);

    if (createMipmap)
    {
        generateMipmap();
    }
}

auto TextureCube::generateMipmap()
    -> void
{
    auto const id = getId();

    glGenerateTextureMipmap(id);
}

auto TextureCube::setLabel(std::string_view const label)
    -> void
{
    auto const id = getId();

    glObjectLabel(GL_TEXTURE, id, static_cast<GLsizei>(label.size()), label.data());
}

} // namespace glow
