#pragma once

#include <Glow/BufferObject/FrameBufferObject.hpp>
#include <Glow/Texture/Texture2dArray.hpp>

#include <string_view>
#include <vector>

namespace ape
{

class MonoDepthMap
{

public:

    MonoDepthMap(basix::Size2d<int> const & size, int numOfLayers, std::string_view label);

    auto getTexture() const
        -> glow::Texture2dArray const &;

    auto getFrameBuffers() const
        -> std::vector<glow::FrameBufferObject> const &;

    auto getSize() const
        -> basix::Size2d<int>;

private:

    glow::Texture2dArray texture;

    std::vector<glow::FrameBufferObject> frameBuffers;

    basix::Size2d<int> size;

};

} // namespace ape
