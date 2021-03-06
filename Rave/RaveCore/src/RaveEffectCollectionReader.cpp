#include <Rave/RaveCore/RaveEffectCollectionReader.hpp>

#include <Ape/Rendering/Effect/EffectCollectionPopulator.hpp>

namespace rave
{

auto RaveEffectCollectionReader::read() const
    -> ape::EffectCollection
{
    auto collection = ape::EffectCollection{};

    auto const populator = ape::EffectCollectionPopulator{collection};

    populator.addAllStandardEffects();

    populator.addAllEffectsInFolder(std::filesystem::path{resourceFolder} / "Effects");

    return collection;
}

} // namespace rave
