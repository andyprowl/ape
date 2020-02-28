#pragma once

#include <Glow/Texture/Image2d.hpp>
#include <Glow/Texture/TextureFiltering.hpp>
#include <Glow/Texture/TextureInternalFormat.hpp>
#include <Glow/Texture/TextureWrapping.hpp>

namespace glow
{

class Texture2dArrayDescriptor
{

public:

    Texture2dArrayDescriptor(
        std::vector<Image2d> layerImages,
        TextureInternalFormat const internalFormat,
        TextureFiltering const filtering,
        TextureWrapping const wrapping,
        int const numOfLayers,
        int const numOfMipmapLevels = 1)
        : imageSet{std::move(imageSet)}
        , internalFormat{internalFormat}
        , filtering{filtering}
        , wrapping{wrapping}
        , numOfLayers{numOfLayers}
        , numOfMipmapLevels{numOfMipmapLevels}
    {
    }

public:

    std::vector<Image2d> layerImages;

    TextureInternalFormat internalFormat;

    TextureFiltering filtering;

    TextureWrapping wrapping;

    int const numOfLayers;

    int const numOfMipmapLevels;

};

} // namespace glow
