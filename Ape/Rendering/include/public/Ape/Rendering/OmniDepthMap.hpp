#pragma once

#include <Ape/GpuResource/CubeTexture.hpp>
#include <Ape/GpuResource/FrameBufferObject.hpp>

namespace ape
{

class OmniDepthMap
{

public:

    explicit OmniDepthMap(Size<int> const & size);

    auto getTexture() const
        -> CubeTexture const &;

    auto getFrameBuffer() const
        -> FrameBufferObject const &;

    auto getSize() const
        -> Size<int>;

private:

    CubeTexture texture;

    FrameBufferObject frameBuffer;

    Size<int> size;

};

} // namespace ape
