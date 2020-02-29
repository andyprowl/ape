#include <Glow/BufferObject/FrameBufferObject.hpp>

#include <Glow/BufferObject/RenderBufferObject.hpp>

#include <Glow/GpuResource/ScopedBinder.hpp>
#include <Glow/Texture/Texture2d.hpp>
#include <Glow/Texture/Texture2dArray.hpp>
#include <Glow/Texture/TextureCube.hpp>
#include <Glow/Texture/TextureCubeArray.hpp>
#include <Glow/Texture/TextureCubeFace.hpp>

#include <glad/glad.h>

#include <cassert>

namespace glow
{

namespace
{

auto getColorAttachmentIndex(FrameBufferAttachment const attachment)
    -> int
{
    auto const index = 
        static_cast<int>(attachment) -
        static_cast<int>(FrameBufferAttachment::color0);

    assert(index >= 0);

    return index;
}

auto convertToOpenGLAttachment(FrameBufferAttachment const attachment)
    -> GLenum 
{
    switch (attachment)
    {
        case FrameBufferAttachment::depth:
        {
            return GL_DEPTH_ATTACHMENT;
        }

        case FrameBufferAttachment::stencil:
        {
            return GL_STENCIL_ATTACHMENT;
        }

        case FrameBufferAttachment::depthAndStencil:
        {
            return GL_DEPTH_STENCIL_ATTACHMENT;
        }

        default:
        {
            return GL_COLOR_ATTACHMENT0 + getColorAttachmentIndex(attachment);
        }
    }
}

auto createFrameBufferResource()
    -> GpuResource
{
    auto id = GpuResource::Id{};

    glCreateFramebuffers(1, &id);

    return GpuResource{id, [] (GpuResource::Id const id) { glDeleteFramebuffers(1, &id); }};
}

} // unnamed namespace

FrameBufferObject::FrameBufferObject()
    : FrameBufferObject{""}
{
}

FrameBufferObject::FrameBufferObject(std::string_view const label)
    : BufferObject{createFrameBufferResource()}
{
    setLabel(label);
}

auto FrameBufferObject::bind() const
    -> void
{
    assert(isValid());

    auto const id = resource.get();

    glBindFramebuffer(GL_FRAMEBUFFER, id);
}

auto FrameBufferObject::unbind() const
    -> void
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

auto FrameBufferObject::isBound() const
    -> bool
{
    auto boundBufferId = 0;

    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &boundBufferId);

    assert(boundBufferId >= 0);

    return (resource.get() == static_cast<unsigned int>(boundBufferId));
}

auto FrameBufferObject::isComplete() const
    -> bool
{
    auto const status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    return (status == GL_FRAMEBUFFER_COMPLETE);
}

auto FrameBufferObject::attach(Texture2d const & texture, FrameBufferAttachment const attachment)
    -> void
{
    auto const framebufferId = getId();

    auto const glAttachment = convertToOpenGLAttachment(attachment);

    auto const textureId = texture.getId();

    auto const mipmapLevel = 0;

    glNamedFramebufferTexture(framebufferId, glAttachment, textureId, mipmapLevel);
}

auto FrameBufferObject::attach(
    Texture2dArray const & texture,
    int const layer,
    FrameBufferAttachment const attachment)
    -> void
{
    auto const framebufferId = getId();

    auto const glAttachment = convertToOpenGLAttachment(attachment);

    auto const textureId = texture.getId();

    auto const mipmapLevel = 0;

    glNamedFramebufferTextureLayer(framebufferId, glAttachment, textureId, mipmapLevel, layer);
}

auto FrameBufferObject::attach(TextureCube const & texture, FrameBufferAttachment const attachment)
    -> void
{
    auto const framebufferId = getId();

    auto const glAttachment = convertToOpenGLAttachment(attachment);

    auto const textureId = texture.getId();

    auto const mipmapLevel = 0;

    glNamedFramebufferTexture(framebufferId, glAttachment, textureId, mipmapLevel);
}

auto FrameBufferObject::attach(
    TextureCube const & texture,
    TextureCubeFace const face,
    FrameBufferAttachment const attachment)
    -> void
{
    auto const framebufferId = getId();

    auto const glAttachment = convertToOpenGLAttachment(attachment);

    auto const layer = static_cast<int>(face);

    auto const textureId = texture.getId();

    auto const mipmapLevel = 0;

    glNamedFramebufferTextureLayer(framebufferId, glAttachment, textureId, mipmapLevel, layer);
}

auto FrameBufferObject::attach(
    TextureCubeArray const & texture,
    int const layer,
    FrameBufferAttachment attachment)
    -> void
{
    auto const framebufferId = getId();

    auto const glAttachment = convertToOpenGLAttachment(attachment);

    auto const layerFace = layer * 6;

    auto const textureId = texture.getId();

    auto const mipmapLevel = 0;

    glNamedFramebufferTextureLayer(framebufferId, glAttachment, textureId, mipmapLevel, layerFace);
}

auto FrameBufferObject::attach(
    TextureCubeArray const & texture,
    int const layer,
    TextureCubeFace face,
    FrameBufferAttachment attachment)
    -> void
{
    auto const framebufferId = getId();

    auto const glAttachment = convertToOpenGLAttachment(attachment);

    auto const layerFace = layer * 6 + static_cast<int>(face);

    auto const textureId = texture.getId();

    auto const mipmapLevel = 0;

    glNamedFramebufferTextureLayer(framebufferId, glAttachment, textureId, mipmapLevel, layerFace);
}

auto FrameBufferObject::attach(
    RenderBufferObject const & renderBuffer,
    FrameBufferAttachment const attachment)
    -> void
{
    auto const framebufferId = getId();

    auto const glAttachment = convertToOpenGLAttachment(attachment);

    auto const bufferId = renderBuffer.getId();

    auto const mipmapLevel = 0;

    glNamedFramebufferRenderbuffer(framebufferId, glAttachment, GL_RENDERBUFFER, bufferId);
}

auto FrameBufferObject::resetReadTarget()
    -> void
{
    auto const id = getId();

    glNamedFramebufferReadBuffer(id, GL_NONE);
}

auto FrameBufferObject::resetDrawTarget()
    -> void
{
    auto const id = getId();

    glNamedFramebufferReadBuffer(id, GL_NONE);
}

auto FrameBufferObject::setLabel(std::string_view const label)
    -> void
{
    // NVidia drivers cause an invalid operation error when setting the label of the framebuffer if
    // it is not bound.
    auto const binder = glow::bind(*this);

    glObjectLabel(GL_FRAMEBUFFER, getId(), static_cast<GLsizei>(label.size()), label.data());
}

} // namespace glow
