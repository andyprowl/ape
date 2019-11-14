#pragma once

#include <Glow/Texture/CubeTexture.hpp>

#include <Basix/Signal/Signal.hpp>

#include <memory>
#include <stdexcept>
#include <vector>

namespace ape
{

class SkyboxNotContained : public std::logic_error
{

public:

    SkyboxNotContained()
        : std::logic_error{"The specified skybox is not a member of the collection"}
    {
    }

};

class SkyboxCollection
{

public:

    SkyboxCollection() = default;

    explicit SkyboxCollection(std::vector<glow::CubeTexture> skyboxes);

    auto addSkybox(glow::CubeTexture skybox)
        -> void;

    auto getNumOfSkyboxes() const
        -> int;

    auto getSkybox(int i)
        -> glow::CubeTexture &;

    auto getSkyboxIndex(glow::CubeTexture const & skybox) const
        -> int;

public:

    basix::Signal<auto (glow::CubeTexture &) -> void> onSkyboxAdded;

private:

    // Held by unique pointer to avoid invalidation issues.
    std::vector<std::unique_ptr<glow::CubeTexture>> skyboxes;

};

} // namespace ape
