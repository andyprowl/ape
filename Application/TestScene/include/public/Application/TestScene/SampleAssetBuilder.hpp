#pragma once

#include <Application/TestScene/SampleAssetCollection.hpp>

class SampleAssetBuilder
{

public:

    auto build() const
        -> SampleAssetCollection;

};

inline auto createSampleAssets()
    -> SampleAssetCollection
{
    auto const builder = SampleAssetBuilder{};

    return builder.build();
}
