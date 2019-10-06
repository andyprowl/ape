#include <GpuResource/FrameBufferObject.hpp>

#include <GpuResource/ScopedBinder.hpp>

#include <glad/glad.h>

#include <cassert>

namespace ape
{

namespace
{

auto createFrameBufferResource()
    -> GpuResource
{
    auto id = 0u;

    glGenFramebuffers(1, &id);

    return GpuResource{id, [] (unsigned int const id) { glDeleteFramebuffers(1, &id); }};
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

} // namespace ape
