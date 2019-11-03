#pragma once

#include <Ape/AssetLoader/AssetRepository.hpp>

namespace rave
{

class RaveAssetCollection
{

public:

    ape::AssetRepository generalAssets;

    ape::AssetRepository nanosuitAssets;

    ape::AssetRepository dragonAssets;

    ape::AssetRepository spaceshipAssets;

    ape::AssetRepository dynoAssets;

    ape::AssetRepository castleAssets;

    std::vector<ape::Shape *> shapes;

};

} // namespace rave
