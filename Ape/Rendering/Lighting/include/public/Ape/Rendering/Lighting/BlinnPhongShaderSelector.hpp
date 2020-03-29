#pragma once

#include <Basix/Signal/Signal.hpp>

#include <optional>
#include <stdexcept>
#include <string>

namespace ape
{

class BlinnPhongShaderCollection;
class BlinnPhongShaderProgram;
class BlinnPhongShaderSelector;

class BadShaderIndex : public std::logic_error
{

public:

    explicit BadShaderIndex(int index)
        : logic_error{"Index " + std::to_string(index) + " is not an available effect index"}
    {
    }

};

class BlinnPhongShaderSelector
{

public:

    explicit BlinnPhongShaderSelector(BlinnPhongShaderCollection & collection);

    auto getCollection() const
        -> BlinnPhongShaderCollection &;

    auto getAvailableShaders() const
        -> std::vector<BlinnPhongShaderProgram *> const &;

    auto getActiveShader() const
        -> BlinnPhongShaderProgram *;
    
    auto activateShader(int index)
        -> void;

    auto activateShader(BlinnPhongShaderProgram const & shader)
        -> void;

    auto activateNextShader()
        -> void;

    auto activatePreviousShader()
        -> void;

    auto deactivateShader()
        -> void;

public:

    basix::Signal<auto (BlinnPhongShaderProgram *) -> void> onActiveShaderChanged;

private:

    auto tryGetFirstShaderIndex() const
        -> std::optional<int>;

    auto tryGetLastShaderIndex() const
        -> std::optional<int>;

private:

    BlinnPhongShaderCollection * collection;

    std::vector<BlinnPhongShaderProgram *> availableShaders;

    std::optional<int> activeShaderIndex;

};

}
