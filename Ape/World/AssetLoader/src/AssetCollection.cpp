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

    collectMaterials(it->second);
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

auto AssetCollection::getMaterials() const
    -> std::vector<Material *> const &
{
    return materials;
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

auto AssetCollection::collectMaterials(AssetRepository & repository)
    -> void
{
    materials.reserve(materials.size() + repository.materials.size());

    for (auto & material : repository.materials)
    {
        materials.push_back(&material);
    }
}

} // namespace ape