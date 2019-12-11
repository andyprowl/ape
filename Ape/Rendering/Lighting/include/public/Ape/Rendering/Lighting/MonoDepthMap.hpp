#pragma once

#include <Glow/BufferObject/FrameBufferObject.hpp>
#include <Glow/Texture/Texture.hpp>

#include <string_view>

namespace ape
{

class MonoDepthMap
{

public:

    explicit MonoDepthMap(basix::Size<int> const & size);

    MonoDepthMap(basix::Size<int> const & size, std::string_view label);

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
