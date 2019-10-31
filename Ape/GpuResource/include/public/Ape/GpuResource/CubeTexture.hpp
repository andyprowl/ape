#pragma once

#include <Ape/GpuResource/GpuResource.hpp>
#include <Ape/GpuResource/CubeTextureDescriptor.hpp>
#include <Ape/GpuResource/TextureWrapping.hpp>

namespace ape
{

class CubeTexture
{

public:

    CubeTexture(CubeTextureDescriptor descriptor, TextureWrapping wrapping);

    auto getId() const
        -> GpuResource::Id;

    auto bind(int unit) const
        -> void;

private:

    GpuResource resource;

};

} // namespace ape
