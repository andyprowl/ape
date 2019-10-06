#include <GpuResource/FrameBufferObject.hpp>

#include <GpuResource/RenderBufferObject.hpp>
#include <GpuResource/ScopedBinder.hpp>
#include <GpuResource/Texture.hpp>

#include <glad/glad.h>

#include <cassert>

namespace ape
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

auto makeConditionalBinder(FrameBufferObject const & object, bool const bind)
    -> ScopedBinder<FrameBufferObject const>
{
    if (bind)
    {
        return ScopedBinder{object};
    }
    else
    {
        assert(object.isBound());

        return ScopedBinder<FrameBufferObject const>{};
    }
}

} // unnamed namespace

FrameBufferObject::FrameBufferObject()
    : BufferObject{createFrameBufferResource()}
{
}

auto FrameBufferObject::bind() const
    -> void
{
    assert(isValid());

    auto const id = resource.get();

    glBindBuffer(GL_FRAMEBUFFER, id);
}

auto FrameBufferObject::unbind() const
    -> void
{
    glBindBuffer(GL_FRAMEBUFFER, 0);
}

auto FrameBufferObject::isBound() const
    -> bool
{
    auto boundBufferId = 0;

    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &boundBufferId);

    assert(boundBufferId >= 0);

    return (resource.get() == static_cast<unsigned int>(boundBufferId));
}

auto FrameBufferObject::isComplete(bool const bind) const
    -> bool
{
    auto const binder = makeConditionalBinder(*this, bind);

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

} // namespace ape
