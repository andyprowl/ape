#include <Ape/Rendering/EffectCollection.hpp>

#include <Foundational/Range/Search.hpp>
#include <Foundational/Range/Transform.hpp>

namespace ape
{

namespace
{

auto makeUniqueEffects(std::vector<EffectShaderProgram> effects)
    -> std::vector<std::unique_ptr<EffectShaderProgram>>
{
    return transform<std::vector>(effects, [] (EffectShaderProgram & effect)
        -> std::unique_ptr<EffectShaderProgram>
    {
        return std::make_unique<EffectShaderProgram>(std::move(effect));
    });
}

} // unnamed namespace

EffectCollection::EffectCollection(std::vector<EffectShaderProgram> effects)
    : effects{makeUniqueEffects(std::move(effects))}
{
}

auto EffectCollection::addEffect(EffectShaderProgram effect)
    -> void
{
    effects.push_back(std::make_unique<EffectShaderProgram>(std::move(effect)));

    onEffectAdded.fire(*effects.back());
}

auto EffectCollection::getNumOfEffects() const
    -> int
{
    return static_cast<int>(effects.size());
}

auto EffectCollection::getEffect(int const i)
    -> EffectShaderProgram &
{
    return *(effects[i]);
}

auto EffectCollection::getEffectIndex(EffectShaderProgram const & effect) const
    -> int
{
    auto const it = findIf(effects, [&effect] (auto const & p)
    {
        return (p.get() == &effect);
    });

    if (it == std::cend(effects))
    {
        throw EffectNotContained{};
    }

    auto const index = std::distance(std::cbegin(effects), it);

    return static_cast<int>(index);
}

} // namespace ape
