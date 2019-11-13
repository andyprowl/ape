#pragma once

#include <Ape/BufferObject/FrameBufferObject.hpp>
#include <Ape/BufferObject/RenderBufferObject.hpp>
#include <Ape/Texture/Texture.hpp>

#include <Foundational/Mathematics/Size.hpp>

namespace ape
{

class OffscreenSurface
{

public:

    explicit OffscreenSurface(Size<int> const & size);

    auto getColorBuffer() const
        -> Texture const &;

    auto getDepthAndStencilBuffer() const
        -> RenderBufferObject const &;

    auto getFrameBuffer() const
        -> FrameBufferObject const &;

    auto getSize() const
        -> Size<int>;

    auto setSize(Size<int> const & newSize)
        -> void;

private:

    Texture colorBuffer;

    RenderBufferObject depthAndStencilBuffer;

    FrameBufferObject frameBuffer;

    Size<int> size;

};

} // namespace RenderQuad
