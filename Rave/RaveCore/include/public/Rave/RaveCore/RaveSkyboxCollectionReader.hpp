#pragma once

#include <Ape/Rendering/SkyboxCollection.hpp>

namespace rave
{

class RaveSkyboxCollectionReader
{

public:

    auto read() const
        -> ape::SkyboxCollection;

};

} // namespace rave
