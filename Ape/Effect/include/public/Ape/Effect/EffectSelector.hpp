#pragma once

#include <Basix/Signal/Signal.hpp>

#include <optional>
#include <stdexcept>
#include <string>

namespace ape
{

class EffectCollection;
class EffectShaderProgram;

class BadEffectIndex : public std::logic_error
{

public:

    explicit BadEffectIndex(int index)
        : logic_error{"Index " + std::to_string(index) + " is not an available effect index"}
    {
    }

};

class EffectSelector
{

public:

    explicit EffectSelector(EffectCollection & collection);

    auto getCollection() const
        -> EffectCollection &;

    auto getAvailableEffects() const
        -> std::vector<EffectShaderProgram *> const &;

    auto getActiveEffect() const
        -> EffectShaderProgram *;
    
    auto activateEffect(int index)
        -> void;

    auto activateEffect(EffectShaderProgram const & effect)
        -> void;

    auto activateNextEffect()
        -> void;

    auto activatePreviousEffect()
        -> void;

    auto deactivateEffect()
        -> void;

public:

    basix::Signal<auto (EffectShaderProgram *) -> void> onActiveEffectChanged;

private:

    auto tryGetFirstEffectIndex() const
        -> std::optional<int>;

    auto tryGetLastEffectIndex() const
        -> std::optional<int>;

private:

    EffectCollection * collection;

    std::vector<EffectShaderProgram *> availableEffects;

    std::optional<int> activeEffectIndex;

};

}
