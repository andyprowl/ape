#pragma once

#include <Ape/GpuResource/CubeTextureReader.hpp>

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

    CubeTextureReader textureReader;

};

} // namespace ape
