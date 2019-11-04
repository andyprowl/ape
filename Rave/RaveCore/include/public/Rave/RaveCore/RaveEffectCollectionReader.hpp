#pragma once

#include <Ape/Rendering/EffectCollection.hpp>

namespace rave
{

class RaveEffectCollectionReader
{

public:

    auto read() const
        -> ape::EffectCollection;

};

} // namespace rave
