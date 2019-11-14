#include <Ape/Rendering/Skybox/SkyboxSelector.hpp>

#include <Ape/Rendering/Skybox/SkyboxCollection.hpp>

#include <Basix/Range/Search.hpp>
#include <Basix/Range/Transform.hpp>

#include <algorithm>
#include <numeric>

namespace ape
{

namespace
{

auto getAllSkyboxesFromCollection(SkyboxCollection & collection)
    -> std::vector<glow::CubeTexture *>
{
    auto const numOfSkyboxes = collection.getNumOfSkyboxes();

    auto effects = std::vector<glow::CubeTexture *>{};

    effects.reserve(numOfSkyboxes);

    for (auto i = 0; i < numOfSkyboxes; ++i)
    {
        auto & effect = collection.getSkybox(i);

        effects.push_back(&effect);
    }

    return effects;
}

} // unnamed namespace

SkyboxSelector::SkyboxSelector(SkyboxCollection & collection)
    : collection{&collection}
    , availableSkyboxes{getAllSkyboxesFromCollection(collection)}
    , activeSkyboxIndex{tryGetFirstSkyboxIndex()}
{
}

auto SkyboxSelector::getCollection() const
    -> SkyboxCollection &
{
    return *collection;
}

auto SkyboxSelector::getAvailableSkyboxes() const
    -> std::vector<glow::CubeTexture *> const &
{
    return availableSkyboxes;
}

auto SkyboxSelector::getActiveSkybox() const
    -> glow::CubeTexture *
{
    if (!activeSkyboxIndex)
    {
        return nullptr;
    }

    return availableSkyboxes[*activeSkyboxIndex];
}

auto SkyboxSelector::activateSkybox(int const index)
    -> void
{
    if (index >= static_cast<int>(availableSkyboxes.size()))
    {
        throw BadSkyboxIndex{index};
    }

    activeSkyboxIndex = index;

    onActiveSkyboxChanged.fire(getActiveSkybox());
}

auto SkyboxSelector::activateSkybox(glow::CubeTexture const & effect)
    -> void
{
    if (getActiveSkybox() == &effect)
    {
        return;
    }

    activeSkyboxIndex = collection->getSkyboxIndex(effect);

    onActiveSkyboxChanged.fire(getActiveSkybox());
}

auto SkyboxSelector::activateNextSkybox()
    -> void
{
    if (!activeSkyboxIndex)
    {
        activeSkyboxIndex = tryGetFirstSkyboxIndex();
    }
    else
    {
        auto const numOfSkyboxes = static_cast<int>(availableSkyboxes.size());

        activeSkyboxIndex = (*activeSkyboxIndex + 1) % numOfSkyboxes;
    }

    onActiveSkyboxChanged.fire(getActiveSkybox());
}

auto SkyboxSelector::activatePreviousSkybox()
    -> void
{
    if (!activeSkyboxIndex)
    {
        activeSkyboxIndex = tryGetLastSkyboxIndex();
    }
    else
    {
        auto const numOfSkyboxes = static_cast<int>(availableSkyboxes.size());

        activeSkyboxIndex = (*activeSkyboxIndex + numOfSkyboxes - 1) % numOfSkyboxes;
    }

    onActiveSkyboxChanged.fire(getActiveSkybox());
}

auto SkyboxSelector::deactivateSkybox()
    -> void
{
    activeSkyboxIndex = std::nullopt;

    onActiveSkyboxChanged.fire(nullptr);
}

auto SkyboxSelector::tryGetFirstSkyboxIndex() const
    -> std::optional<int>
{
    if (availableSkyboxes.empty())
    {
        return std::nullopt;
    }

    return 0;
}

auto SkyboxSelector::tryGetLastSkyboxIndex() const
    -> std::optional<int>
{
    if (availableSkyboxes.empty())
    {
        return std::nullopt;
    }

    return static_cast<int>(availableSkyboxes.size() - 1);
}

} // namespace ape
