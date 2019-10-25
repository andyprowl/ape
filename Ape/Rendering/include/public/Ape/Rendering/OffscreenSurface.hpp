#pragma once

#include <Ape/GpuResource/FrameBufferObject.hpp>
#include <Ape/GpuResource/RenderBufferObject.hpp>
#include <Ape/GpuResource/Texture.hpp>

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

private:

    Texture colorBuffer;

    RenderBufferObject depthAndStencilBuffer;

    FrameBufferObject frameBuffer;

    Size<int> size;

};

} // namespace RenderQuad
