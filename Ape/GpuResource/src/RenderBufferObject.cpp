#include <GpuResource/RenderBufferObject.hpp>

#include <GpuResource/ScopedBinder.hpp>

#include <glad/glad.h>

#include <cassert>

namespace ape
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

RenderBufferObject::RenderBufferObject(Size<int> const size)
    : BufferObject{createRenderBufferResource()}
{
    setStorage(size);
}

auto RenderBufferObject::bind() const
    -> void
{
    assert(isValid());

    auto const id = resource.get();

    glBindBuffer(GL_RENDERBUFFER, id);
}

auto RenderBufferObject::unbind() const
    -> void
{
    glBindBuffer(GL_RENDERBUFFER, 0);
}

auto RenderBufferObject::setStorage(Size<int> const size)
    -> void
{
    auto const binder = ScopedBinder{*this};

    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, size.width, size.height);
}

} // namespace ape