#pragma once

#include <Ape/GpuResource/TextureImage.hpp>

namespace ape
{

class CubeTextureImageSet
{

public:

    CubeTextureImageSet(
        TextureImage const & right,
        TextureImage const & left,
        TextureImage const & top,
        TextureImage const & bottom,
        TextureImage const & front,
        TextureImage const & back)
        : right{right}
        , left{left}
        , top{top}
        , bottom{bottom}
        , front{front}
        , back{back}
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

} // namespace ape
