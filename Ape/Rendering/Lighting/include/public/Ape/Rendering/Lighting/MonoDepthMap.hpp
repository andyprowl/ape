#pragma once

#include <Glow/BufferObject/FrameBufferObject.hpp>
#include <Glow/Texture/Texture.hpp>

namespace ape
{

class MonoDepthMap
{

public:

    explicit MonoDepthMap(basix::Size<int> const & size);

    auto getTexture() const
        -> glow::Texture const &;

    auto getFrameBuffer() const
        -> glow::FrameBufferObject const &;

    auto getSize() const
        -> basix::Size<int>;

private:

    glow::Texture texture;

    glow::FrameBufferObject frameBuffer;

    basix::Size<int> size;

};

} // namespace ape
