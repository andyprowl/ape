#pragma once

#include <Basix/Signal/Signal.hpp>

#include <optional>
#include <stdexcept>
#include <string>

namespace glow
{

class CubeTexture;

} // namespace glow

namespace ape
{

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
        -> std::vector<glow::CubeTexture *> const &;

    auto getActiveSkybox() const
        -> glow::CubeTexture *;
    
    auto activateSkybox(int index)
        -> void;

    auto activateSkybox(glow::CubeTexture const & collection)
        -> void;

    auto activateNextSkybox()
        -> void;

    auto activatePreviousSkybox()
        -> void;

    auto deactivateSkybox()
        -> void;

public:

    basix::Signal<auto (glow::CubeTexture *) -> void> onActiveSkyboxChanged;

private:

    auto tryGetFirstSkyboxIndex() const
        -> std::optional<int>;

    auto tryGetLastSkyboxIndex() const
        -> std::optional<int>;

private:

    SkyboxCollection * collection;

    std::vector<glow::CubeTexture *> availableSkyboxes;

    std::optional<int> activeSkyboxIndex;

};

}
