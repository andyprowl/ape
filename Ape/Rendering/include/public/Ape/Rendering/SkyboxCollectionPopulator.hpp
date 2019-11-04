#pragma once

#include <Ape/GpuResource/TextureReader.hpp>

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

    TextureReader textureReader;

};

} // namespace ape
