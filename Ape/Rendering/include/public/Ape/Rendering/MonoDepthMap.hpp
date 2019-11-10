#pragma once

#include <Ape/GpuResource/FrameBufferObject.hpp>
#include <Ape/GpuResource/Texture.hpp>

namespace ape
{

class MonoDepthMap
{

public:

    explicit MonoDepthMap(Size<int> const & size);

    auto getTexture() const
        -> Texture const &;

    auto getFrameBuffer() const
        -> FrameBufferObject const &;

    auto getSize() const
        -> Size<int>;

private:

    Texture texture;

    FrameBufferObject frameBuffer;

    Size<int> size;

};

} // namespace ape
