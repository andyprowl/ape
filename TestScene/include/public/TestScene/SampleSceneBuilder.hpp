#pragma once

#include <TestScene/SampleScene.hpp>

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

inline auto createSampleScene(SampleAssetCollection & assets)
    -> SampleScene
{
    auto const builder = SampleSceneBuilder{assets};

    return builder.build();
}
