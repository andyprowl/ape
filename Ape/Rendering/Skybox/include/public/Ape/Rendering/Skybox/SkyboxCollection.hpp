#pragma once

#include <Glow/Texture/TextureCube.hpp>

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

    explicit SkyboxCollection(std::vector<glow::TextureCube> skyboxes);

    auto addSkybox(glow::TextureCube skybox)
        -> void;

    auto getNumOfSkyboxes() const
        -> int;

    auto getSkybox(int i)
        -> glow::TextureCube &;

    auto getSkyboxIndex(glow::TextureCube const & skybox) const
        -> int;

public:

    basix::Signal<auto (glow::TextureCube &) -> void> onSkyboxAdded;

private:

    // Held by unique pointer to avoid invalidation issues.
    std::vector<std::unique_ptr<glow::TextureCube>> skyboxes;

};

} // namespace ape
