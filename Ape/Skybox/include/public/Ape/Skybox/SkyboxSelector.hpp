#pragma once

#include <Foundational/Signal/Signal.hpp>

#include <optional>
#include <stdexcept>
#include <string>

namespace ape
{

class CubeTexture;
class SkyboxCollection;

class BadSkyboxIndex : public std::logic_error
{

public:

    explicit BadSkyboxIndex(int index)
        : logic_error{"Index " + std::to_string(index) + " is not an available collection index"}
    {
    }

};

class SkyboxSelector
{

public:

    explicit SkyboxSelector(SkyboxCollection & collection);

    auto getCollection() const
        -> SkyboxCollection &;

    auto getAvailableSkyboxes() const
        -> std::vector<CubeTexture *> const &;

    auto getActiveSkybox() const
        -> CubeTexture *;
    
    auto activateSkybox(int index)
        -> void;

    auto activateSkybox(CubeTexture const & collection)
        -> void;

    auto activateNextSkybox()
        -> void;

    auto activatePreviousSkybox()
        -> void;

    auto deactivateSkybox()
        -> void;

public:

    Signal<auto (CubeTexture *) -> void> onActiveSkyboxChanged;

private:

    auto tryGetFirstSkyboxIndex() const
        -> std::optional<int>;

    auto tryGetLastSkyboxIndex() const
        -> std::optional<int>;

private:

    SkyboxCollection * collection;

    std::vector<CubeTexture *> availableSkyboxes;

    std::optional<int> activeSkyboxIndex;

};

}
