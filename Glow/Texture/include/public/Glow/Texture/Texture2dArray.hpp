#pragma once

#include <Glow/Texture/Texture2dDescriptor.hpp>

#include <Glow/GpuResource/GpuResource.hpp>

#include <string_view>

namespace glow
{

class Image2d;

class Texture2dArray
{

public:

    Texture2dArray(
        Texture2dDescriptor const & layerDescriptor,
        int numOfLayers,
        std::string_view label = "");

    auto getId() const
        -> GpuResource::Id;

    auto bind() const
        -> void;

    auto unbind() const
        -> void;

    auto getLayerDescriptor() const
        -> Texture2dDescriptor const &;

    auto setLayerImage(int layer, Image2d const & image)
        -> void;

    auto generateMipmap()
        -> void;

    auto setLabel(std::string_view const label)
        -> void;

private:

    GpuResource resource;

    Texture2dDescriptor layerDescriptor;

    int numOfLayers;

};

} // namespace glow
