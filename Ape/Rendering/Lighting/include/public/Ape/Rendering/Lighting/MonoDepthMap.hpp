#pragma once

#include <Glow/BufferObject/FrameBufferObject.hpp>
#include <Glow/Texture/Texture2d.hpp>

#include <string_view>

namespace ape
{

class MonoDepthMap
{

public:

    explicit MonoDepthMap(basix::Size2d<int> const & size);

    MonoDepthMap(basix::Size2d<int> const & size, std::string_view label);

    auto getTexture() const
        -> glow::Texture2d const &;

    auto getFrameBuffer() const
        -> glow::FrameBufferObject const &;

    auto getSize() const
        -> basix::Size2d<int>;

private:

    glow::Texture2d texture;

    glow::FrameBufferObject frameBuffer;

    basix::Size2d<int> size;

};

} // namespace ape
