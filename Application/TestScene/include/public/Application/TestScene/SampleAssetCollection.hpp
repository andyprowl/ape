#pragma once

#include <Ape/AssetLoader/AssetRepository.hpp>

class SampleAssetCollection
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
