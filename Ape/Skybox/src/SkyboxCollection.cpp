#include <Ape/Skybox/SkyboxCollection.hpp>

#include <Foundational/Range/Search.hpp>
#include <Foundational/Range/Transform.hpp>

namespace ape
{

namespace
{

auto makeUniqueSkyboxes(std::vector<CubeTexture> skyboxes)
    -> std::vector<std::unique_ptr<CubeTexture>>
{
    return transform<std::vector>(skyboxes, [] (CubeTexture & skybox)
        -> std::unique_ptr<CubeTexture>
    {
        return std::make_unique<CubeTexture>(std::move(skybox));
    });
}

} // unnamed namespace

SkyboxCollection::SkyboxCollection(std::vector<CubeTexture> skyboxes)
    : skyboxes{makeUniqueSkyboxes(std::move(skyboxes))}
{
}

auto SkyboxCollection::addSkybox(CubeTexture skybox)
    -> void
{
    skyboxes.push_back(std::make_unique<CubeTexture>(std::move(skybox)));

    onSkyboxAdded.fire(*skyboxes.back());
}

auto SkyboxCollection::getNumOfSkyboxes() const
    -> int
{
    return static_cast<int>(skyboxes.size());
}

auto SkyboxCollection::getSkybox(int const i)
    -> CubeTexture &
{
    return *(skyboxes[i]);
}

auto SkyboxCollection::getSkyboxIndex(CubeTexture const & skybox) const
    -> int
{
    auto const it = findIf(skyboxes, [&skybox] (auto const & p)
    {
        return (p.get() == &skybox);
    });

    if (it == std::cend(skyboxes))
    {
        throw SkyboxNotContained{};
    }

    auto const index = std::distance(std::cbegin(skyboxes), it);

    return static_cast<int>(index);
}

} // namespace ape
