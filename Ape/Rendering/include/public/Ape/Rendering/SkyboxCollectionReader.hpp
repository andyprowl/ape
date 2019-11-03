#pragma once

#include <Ape/Rendering/SkyboxCollection.hpp>

#include <Ape/GpuResource/TextureReader.hpp>

#include <filesystem>

namespace ape
{

class SkyboxCollectionReader
{

public:

    auto read(std::filesystem::path const & skyboxFolder) const
        -> SkyboxCollection;

};

} // namespace ape
