#pragma once

#include <Ape/Rendering/SkyboxCollection.hpp>

#include <Ape/GpuResource/TextureReader.hpp>

#include <filesystem>

namespace ape
{

class SkyboxCollectionReader
{

public:

    SkyboxCollectionReader();

    auto read() const
        -> SkyboxCollection;

private:

    std::filesystem::path skyboxFolder;

};

} // namespace ape
