#pragma once

#include <Glow/Texture/Image2d.hpp>

namespace glow
{

class TextureCubeImageSet
{

public:

    TextureCubeImageSet(
        Image2d right,
        Image2d left,
        Image2d top,
        Image2d bottom,
        Image2d front,
        Image2d back)
        : right{std::move(right)}
        , left{std::move(left)}
        , top{std::move(top)}
        , bottom{std::move(bottom)}
        , front{std::move(front)}
        , back{std::move(back)}
    {
    }

public:

    Image2d right;

    Image2d left;

    Image2d top;

    Image2d bottom;

    Image2d front;

    Image2d back;

};

} // namespace glow
