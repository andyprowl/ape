#pragma once

#include <Glow/BufferObject/FrameBufferObject.hpp>
#include <Glow/Texture/CubeTexture.hpp>

namespace ape
{

class OmniDepthMap
{

public:

    explicit OmniDepthMap(basix::Size<int> const & size);

    auto getTexture()
        -> glow::CubeTexture &;

    auto getTexture() const
        -> glow::CubeTexture const &;

    auto getFrameBuffer()
        -> glow::FrameBufferObject &;

    auto getFrameBuffer() const
        -> glow::FrameBufferObject const &;

    auto getSize() const
        -> basix::Size<int>;

private:

    glow::CubeTexture texture;

    glow::FrameBufferObject frameBuffer;

    basix::Size<int> size;

};

} // namespace ape
