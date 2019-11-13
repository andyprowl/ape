#pragma once

#include <Ape/Texture/CubeTexture.hpp>

#include <Foundational/Signal/Signal.hpp>

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

    explicit SkyboxCollection(std::vector<CubeTexture> skyboxes);

    auto addSkybox(CubeTexture skybox)
        -> void;

    auto getNumOfSkyboxes() const
        -> int;

    auto getSkybox(int i)
        -> CubeTexture &;

    auto getSkyboxIndex(CubeTexture const & skybox) const
        -> int;

public:

    Signal<auto (CubeTexture &) -> void> onSkyboxAdded;

private:

    // Held by unique pointer to avoid invalidation issues.
    std::vector<std::unique_ptr<CubeTexture>> skyboxes;

};

} // namespace ape
