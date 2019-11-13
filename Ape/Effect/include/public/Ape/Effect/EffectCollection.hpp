#pragma once

#include <Ape/Effect/EffectShaderProgram.hpp>

#include <Foundational/Signal/Signal.hpp>

#include <memory>
#include <stdexcept>
#include <vector>

namespace ape
{

class EffectNotContained : public std::logic_error
{

public:

    EffectNotContained()
        : std::logic_error{"The specified effect is not a member of the collection"}
    {
    }

};

class EffectCollection
{

public:

    EffectCollection() = default;

    explicit EffectCollection(std::vector<EffectShaderProgram> effects);

    auto addEffect(EffectShaderProgram effect)
        -> void;

    auto getNumOfEffects() const
        -> int;

    auto getEffect(int i)
        -> EffectShaderProgram &;

    auto getEffectIndex(EffectShaderProgram const & effect) const
        -> int;

public:

    Signal<auto (EffectShaderProgram &) -> void> onEffectAdded;

private:

    // Held by unique pointer to avoid invalidation issues.
    std::vector<std::unique_ptr<EffectShaderProgram>> effects;

};

} // namespace ape
