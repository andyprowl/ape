#pragma once

#include <Ape/Skybox/SkyboxCollection.hpp>

namespace rave
{

class RaveSkyboxCollectionReader
{

public:

    auto read() const
        -> ape::SkyboxCollection;

};

} // namespace rave
