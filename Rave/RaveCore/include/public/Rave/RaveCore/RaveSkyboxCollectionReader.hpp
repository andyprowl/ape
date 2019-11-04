#pragma once

#include <Ape/Rendering/SkyboxCollection.hpp>
#include <Ape/Rendering/SkyboxCollectionReader.hpp>

namespace rave
{

class RaveSkyboxCollectionReader
{

public:

    auto read() const
        -> ape::SkyboxCollection;

private:

    ape::SkyboxCollectionReader reader;

};

} // namespace rave
