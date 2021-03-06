#pragma once

#include <Glow/Texture/TextureSwizzle.hpp>

#include <array>

namespace glow
{

class TextureSwizzleMask
{

public:

    TextureSwizzleMask(
        const TextureSwizzle redChannelSwizzle,
        const TextureSwizzle greenChannelSwizzle,
        const TextureSwizzle blueChannelSwizzle,
        const TextureSwizzle alphaChannelSwizzle)
        : red{redChannelSwizzle}
        , green{greenChannelSwizzle}
        , blue{blueChannelSwizzle}
        , alpha{alphaChannelSwizzle}
    {
    }

public:

    TextureSwizzle red;

    TextureSwizzle green;

    TextureSwizzle blue;

    TextureSwizzle alpha;

};

auto convertToOpenGLSwizzleMask(TextureSwizzleMask const & mask)
    -> std::array<GLint, 4u>;

} // namespace glow
