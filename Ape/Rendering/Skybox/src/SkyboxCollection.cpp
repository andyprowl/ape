#include <Ape/Rendering/Skybox/SkyboxCollection.hpp>

#include <Basix/Range/Search.hpp>
#include <Basix/Range/Transform.hpp>

namespace ape
{

namespace
{

auto makeUniqueSkyboxes(std::vector<glow::TextureCube> skyboxes)
    -> std::vector<std::unique_ptr<glow::TextureCube>>
{
    return basix::transform(skyboxes, [] (glow::TextureCube & skybox)
        -> std::unique_ptr<glow::TextureCube>
    {
        return std::make_unique<glow::TextureCube>(std::move(skybox));
    });
}

} // unnamed namespace

SkyboxCollection::SkyboxCollection(std::vector<glow::TextureCube> skyboxes)
    : skyboxes{makeUniqueSkyboxes(std::move(skyboxes))}
{
}

auto SkyboxCollection::addSkybox(glow::TextureCube skybox)
    -> void
{
    skyboxes.push_back(std::make_unique<glow::TextureCube>(std::move(skybox)));

    onSkyboxAdded.fire(*skyboxes.back());
}

auto SkyboxCollection::getNumOfSkyboxes() const
    -> int
{
    return static_cast<int>(skyboxes.size());
}

auto SkyboxCollection::getSkybox(int const i)
    -> glow::TextureCube &
{
    return *(skyboxes[i]);
}

auto SkyboxCollection::getSkyboxIndex(glow::TextureCube const & skybox) const
    -> int
{
    auto const it = basix::findIf(skyboxes, [&skybox] (auto const & p)
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
