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

    glCreateRenderbuffers(1, &id);

    return GpuResource{id, [] (GpuResource::Id const id) { glDeleteRenderbuffers(1, &id); }};
}

} // unnamed namespace

RenderBufferObject::RenderBufferObject(basix::Size<int> const size)
    : RenderBufferObject{size, ""}
{
}

RenderBufferObject::RenderBufferObject(basix::Size<int> const size, std::string_view const label)
    : BufferObject{createRenderBufferResource()}
{
    setStorage(size);

    setLabel(label);
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
    glNamedRenderbufferStorage(getId(), GL_DEPTH24_STENCIL8, size.width, size.height);
}

auto RenderBufferObject::setLabel(std::string_view const label)
    -> void
{
    glObjectLabel(GL_RENDERBUFFER, getId(), static_cast<GLsizei>(label.size()), label.data());
}

} // namespace glow
