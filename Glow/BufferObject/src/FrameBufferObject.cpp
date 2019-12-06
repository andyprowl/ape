#include <Glow/BufferObject/FrameBufferObject.hpp>

#include <Glow/BufferObject/RenderBufferObject.hpp>

#include <Glow/Texture/CubeTexture.hpp>
#include <Glow/Texture/CubeTextureFace.hpp>
#include <Glow/Texture/Texture.hpp>

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

    glGenFramebuffers(1, &id);

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

auto FrameBufferObject::attach(Texture const & texture, FrameBufferAttachment const attachment)
    -> void
{
    auto const glAttachment = convertToOpenGLAttachment(attachment);

    auto const textureId = texture.getId();

    auto const mipmapLevel = 0;

    glFramebufferTexture2D(GL_FRAMEBUFFER, glAttachment, GL_TEXTURE_2D, textureId, mipmapLevel);
}

auto FrameBufferObject::attach(CubeTexture const & texture, FrameBufferAttachment const attachment)
    -> void
{
    auto const glAttachment = convertToOpenGLAttachment(attachment);

    auto const textureId = texture.getId();

    auto const mipmapLevel = 0;

    glFramebufferTexture(GL_FRAMEBUFFER, glAttachment, textureId, mipmapLevel);
}

auto FrameBufferObject::attach(
    CubeTexture const & texture,
    CubeTextureFace const face,
    FrameBufferAttachment const attachment)
    -> void
{
    auto const glAttachment = convertToOpenGLAttachment(attachment);

    auto const glFace = convertToOpenGLFace(face);

    auto const textureId = texture.getId();

    auto const mipmapLevel = 0;

    glFramebufferTexture2D(GL_FRAMEBUFFER, glAttachment, glFace, textureId, mipmapLevel);
}

auto FrameBufferObject::attach(
    RenderBufferObject const & renderBuffer,
    FrameBufferAttachment const attachment)
    -> void
{
    auto const glAttachment = convertToOpenGLAttachment(attachment);

    auto const bufferId = renderBuffer.getId();

    auto const mipmapLevel = 0;

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, glAttachment, GL_RENDERBUFFER, bufferId);
}

auto FrameBufferObject::resetReadTarget()
    -> void
{
    glReadBuffer(GL_NONE);
}

auto FrameBufferObject::resetDrawTarget()
    -> void
{
    glDrawBuffer(GL_NONE);
}

auto FrameBufferObject::setLabel(std::string_view const label)
    -> void
{
    glObjectLabel(GL_FRAMEBUFFER, getId(), static_cast<GLsizei>(label.size()), label.data());
}

} // namespace glow
