#pragma once

#include <Glow/Texture/TextureImage.hpp>

namespace glow
{

class CubeTextureImageSet
{

public:

    CubeTextureImageSet(
        TextureImage right,
        TextureImage left,
        TextureImage top,
        TextureImage bottom,
        TextureImage front,
        TextureImage back)
        : right{std::move(right)}
        , left{std::move(left)}
        , top{std::move(top)}
        , bottom{std::move(bottom)}
        , front{std::move(front)}
        , back{std::move(back)}
    {
    }

public:

    TextureImage right;

    TextureImage left;

    TextureImage top;

    TextureImage bottom;

    TextureImage front;

    TextureImage back;

};

} // namespace glow
