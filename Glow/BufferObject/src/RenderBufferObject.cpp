#include <Glow/BufferObject/RenderBufferObject.hpp>

#include <Glow/GpuResource/ScopedBinder.hpp>

#include <glad/glad.h>

#include <cassert>

namespace glow
{

namespace
{

auto createRenderBufferResource()
    -> GpuResource
{
    auto id = GpuResource::Id{};

    glGenRenderbuffers(1, &id);

    return GpuResource{id, [] (GpuResource::Id const id) { glDeleteRenderbuffers(1, &id); }};
}

} // unnamed namespace

RenderBufferObject::RenderBufferObject(basix::Size<int> const size)
    : BufferObject{createRenderBufferResource()}
{
    setStorage(size);
}

auto RenderBufferObject::bind() const
    -> void
{
    assert(isValid());

    auto const id = resource.get();

    glBindRenderbuffer(GL_RENDERBUFFER, id);
}

auto RenderBufferObject::unbind() const
    -> void
{
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

auto RenderBufferObject::setStorage(basix::Size<int> const size)
    -> void
{
    auto const binder = glow::bind(*this);

    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, size.width, size.height);
}

} // namespace glow
