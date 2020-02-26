#pragma once

#include <Glow/BufferObject/FrameBufferObject.hpp>
#include <Glow/Texture/TextureCube.hpp>

#include <string_view>

namespace ape
{

class OmniDepthMap
{

public:

    explicit OmniDepthMap(basix::Size2d<int> const & size);

    OmniDepthMap(basix::Size2d<int> const & size, std::string_view const label);

    auto getTexture()
        -> glow::TextureCube &;

    auto getTexture() const
        -> glow::TextureCube const &;

    auto getFrameBuffer()
        -> glow::FrameBufferObject &;

    auto getFrameBuffer() const
        -> glow::FrameBufferObject const &;

    auto getSize() const
        -> basix::Size2d<int>;

private:

    glow::TextureCube texture;

    glow::FrameBufferObject frameBuffer;

    basix::Size2d<int> size;

};

} // namespace ape
