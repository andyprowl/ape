#pragma once

#include <Ape/World/AssetLoader//AssetRepository.hpp>

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

    ape::AssetRepository tavernAssets;

    ape::AssetRepository houseAssets;

    ape::AssetRepository cottageAssets;

    ape::AssetRepository bridgeAssets;

    ape::AssetRepository libertyStatueAssets;

    ape::AssetRepository sponzaAssets;

    std::vector<ape::Shape *> shapes;

};

} // namespace rave
