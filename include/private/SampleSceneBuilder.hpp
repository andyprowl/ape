#pragma once

#include "SampleScene.hpp"

class AssetRepository;

class SampleSceneBuilder
{

public:

    explicit SampleSceneBuilder(AssetRepository & repository);

    auto build() const
        -> SampleScene;

private:

    AssetRepository * repository;

};
