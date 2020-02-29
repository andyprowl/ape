#pragma once

#include <Glow/Texture/Texture2dDescriptor.hpp>

#include <Glow/GpuResource/GpuResource.hpp>

#include <string_view>

namespace glow
{

class TextureCubeImageSet;

class TextureCubeArray
{

public:

    TextureCubeArray(
        Texture2dDescriptor const & layerFaceDescriptor,
        int numOfLayers,
        std::string_view label = "");

    auto getId() const
        -> GpuResource::Id;

    auto bind() const
        -> void;

    auto unbind() const
        -> void;

    auto getLayerFaceDescriptor() const
        -> Texture2dDescriptor const &;

    auto setLayerImages(int layer, TextureCubeImageSet const & images)
        -> void;

    auto generateMipmap()
        -> void;

    auto setLabel(std::string_view const label)
        -> void;

private:

    GpuResource resource;

    Texture2dDescriptor layerFaceDescriptor;

    int numOfLayers;

};

} // namespace glow
