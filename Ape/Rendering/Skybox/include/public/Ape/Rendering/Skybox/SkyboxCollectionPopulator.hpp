#pragma once

#include <Glow/Texture/CubeTextureReader.hpp>

#include <filesystem>

namespace ape
{

class SkyboxCollection;

class SkyboxCollectionPopulator
{

public:

    explicit SkyboxCollectionPopulator(SkyboxCollection & collection);

    auto addAllSkyboxesInFolder(std::filesystem::path const & skyboxFolder) const
        -> void;

private:

    SkyboxCollection * collection;

    glow::CubeTextureReader textureReader;

};

} // namespace ape
