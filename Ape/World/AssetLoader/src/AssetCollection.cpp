#include <Ape/World/AssetLoader/AssetCollection.hpp>

namespace ape
{

auto AssetCollection::addAssets(std::string name, AssetRepository repository)
    -> void
{
    auto const result = assetMap.try_emplace(name, std::move(repository));

    if (not result.second)
    {
        throw AssetRepositoryNameNotUnique{std::move(name)};
    }

    auto const it = result.first;

    collectShapes(it->second);
}

auto AssetCollection::getAssets(std::string const & name)
    -> AssetRepository &
{
    return assetMap.at(name);
}

auto AssetCollection::getShapes() const
    -> std::vector<Shape *> const &
{
    return shapes;
}

auto AssetCollection::collectShapes(AssetRepository & repository)
    -> void
{
    shapes.reserve(shapes.size() + repository.shapes.size());

    for (auto & shape : repository.shapes)
    {
        shapes.push_back(&shape);
    }
}

} // namespace ape