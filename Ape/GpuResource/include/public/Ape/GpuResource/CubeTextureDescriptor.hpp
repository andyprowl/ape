#pragma once

#include <Ape/GpuResource/TextureDescriptor.hpp>

namespace ape
{

class CubeTextureDescriptor
{

public:

    CubeTextureDescriptor(
        TextureDescriptor right,
        TextureDescriptor left,
        TextureDescriptor top,
        TextureDescriptor bottom,
        TextureDescriptor front,
        TextureDescriptor back)
        : right{std::move(right)}
        , left{std::move(left)}
        , top{std::move(top)}
        , bottom{std::move(bottom)}
        , front{std::move(front)}
        , back{std::move(back)}
    {
    }

public:

    TextureDescriptor right;

    TextureDescriptor left;

    TextureDescriptor top;

    TextureDescriptor bottom;

    TextureDescriptor front;

    TextureDescriptor back;

};

} // namespace ape
