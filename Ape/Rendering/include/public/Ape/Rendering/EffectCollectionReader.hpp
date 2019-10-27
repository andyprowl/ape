#pragma once

#include <Ape/Rendering/EffectCollection.hpp>

#include <filesystem>

namespace ape
{

class EffectCollectionReader
{

public:

    auto read() const
        -> EffectCollection;

};

} // namespace ape
