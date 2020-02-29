#pragma once

#include <Glow/BufferObject/FrameBufferObject.hpp>
#include <Glow/Texture/TextureCubeArray.hpp>

#include <string_view>
#include <vector>

namespace ape
{

class OmniDepthMap
{

public:

    OmniDepthMap(basix::Size2d<int> const & size, int numOfLayers, std::string_view label);

    auto getTexture() const
        -> glow::TextureCubeArray const &;

    auto getFrameBuffers() const
        -> std::vector<glow::FrameBufferObject> const &;

    auto getFrameBuffer(int layer, glow::TextureCubeFace face) const
        -> glow::FrameBufferObject const &;

    auto getSize() const
        -> basix::Size2d<int>;

private:

    glow::TextureCubeArray texture;

    std::vector<glow::FrameBufferObject> frameBuffers;

    basix::Size2d<int> size;

};

} // namespace ape
