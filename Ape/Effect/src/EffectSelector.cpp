#include <Ape/Effect/EffectSelector.hpp>

#include <Ape/Effect/EffectCollection.hpp>

namespace ape
{

namespace
{

auto getAllEffectsFromCollection(EffectCollection & collection)
    -> std::vector<EffectShaderProgram *>
{
    auto const numOfEffects = collection.getNumOfEffects();

    auto effects = std::vector<EffectShaderProgram *>{};

    effects.reserve(numOfEffects);

    for (auto i = 0; i < numOfEffects; ++i)
    {
        auto & effect = collection.getEffect(i);

        effects.push_back(&effect);
    }

    return effects;
}

} // unnamed namespace

EffectSelector::EffectSelector(EffectCollection & collection)
    : collection{&collection}
    , availableEffects{getAllEffectsFromCollection(collection)}
    , activeEffectIndex{tryGetFirstEffectIndex()}
{
}

auto EffectSelector::getCollection() const
    -> EffectCollection &
{
    return *collection;
}

auto EffectSelector::getAvailableEffects() const
    -> std::vector<EffectShaderProgram *> const &
{
    return availableEffects;
}

auto EffectSelector::getActiveEffect() const
    -> EffectShaderProgram *
{
    if (!activeEffectIndex)
    {
        return nullptr;
    }

    return availableEffects[*activeEffectIndex];
}

auto EffectSelector::activateEffect(int const index)
    -> void
{
    if (index >= static_cast<int>(availableEffects.size()))
    {
        throw BadEffectIndex{index};
    }

    activeEffectIndex = index;

    onActiveEffectChanged.fire(getActiveEffect());
}

auto EffectSelector::activateEffect(EffectShaderProgram const & effect)
    -> void
{
    if (getActiveEffect() == &effect)
    {
        return;
    }

    activeEffectIndex = collection->getEffectIndex(effect);

    onActiveEffectChanged.fire(getActiveEffect());
}

auto EffectSelector::activateNextEffect()
    -> void
{
    if (!activeEffectIndex)
    {
        activeEffectIndex = tryGetFirstEffectIndex();
    }
    else
    {
        auto const numOfEffects = static_cast<int>(availableEffects.size());

        activeEffectIndex = (*activeEffectIndex + 1) % numOfEffects;
    }

    onActiveEffectChanged.fire(getActiveEffect());
}

auto EffectSelector::activatePreviousEffect()
    -> void
{
    if (!activeEffectIndex)
    {
        activeEffectIndex = tryGetLastEffectIndex();
    }
    else
    {
        auto const numOfEffects = static_cast<int>(availableEffects.size());

        activeEffectIndex = (*activeEffectIndex + numOfEffects - 1) % numOfEffects;
    }

    onActiveEffectChanged.fire(getActiveEffect());
}

auto EffectSelector::deactivateEffect()
    -> void
{
    activeEffectIndex = std::nullopt;

    onActiveEffectChanged.fire(nullptr);
}

auto EffectSelector::tryGetFirstEffectIndex() const
    -> std::optional<int>
{
    if (availableEffects.empty())
    {
        return std::nullopt;
    }

    return 0;
}

auto EffectSelector::tryGetLastEffectIndex() const
    -> std::optional<int>
{
    if (availableEffects.empty())
    {
        return std::nullopt;
    }

    return static_cast<int>(availableEffects.size() - 1);
}

} // namespace ape
