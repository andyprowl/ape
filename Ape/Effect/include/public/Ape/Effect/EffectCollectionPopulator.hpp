#pragma once

#include <filesystem>

namespace ape
{

class EffectCollection;

class EffectCollectionPopulator
{

public:

    explicit EffectCollectionPopulator(EffectCollection & collection);

    auto addAllEffectsInFolder(std::filesystem::path const & folderPath) const
        -> void;

    auto addAllStandardEffects() const
        -> void;

private:

    EffectCollection * collection;

};

} // namespace ape
