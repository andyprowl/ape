#pragma once

#include <Glow/Texture/Texture2dDescriptor.hpp>

#include <Glow/GpuResource/GpuResource.hpp>

namespace glow
{

enum class TextureStorageType;

class TextureCubeImageSet;

class TextureCube
{

public:

    explicit TextureCube(Texture2dDescriptor const & faceDescriptor);

    TextureCube(Texture2dDescriptor const & faceDescriptor, std::string_view label);

    auto getId() const
        -> GpuResource::Id;

    auto bind() const
        -> void;

    auto unbind() const
        -> void;

    auto getFaceDescriptor() const
        -> Texture2dDescriptor const &;

    auto setFaceImages(TextureCubeImageSet const & images, bool createMipmap)
        -> void;

    auto generateMipmap()
        -> void;

    auto setLabel(std::string_view label)
        -> void;

private:

    GpuResource resource;

    Texture2dDescriptor faceDescriptor;

};

} // namespace glow
