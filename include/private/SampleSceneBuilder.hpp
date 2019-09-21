#pragma once

#include "SampleScene.hpp"

class SampleAssetCollection;

class SampleSceneBuilder
{

public:

    explicit SampleSceneBuilder(SampleAssetCollection & assets);

    auto build() const
        -> SampleScene;

private:

    SampleAssetCollection * assets;

};
