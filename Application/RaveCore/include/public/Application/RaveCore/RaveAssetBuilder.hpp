#pragma once

#include <Application/RaveCore/RaveAssetCollection.hpp>

namespace rave
{

class RaveAssetBuilder
{

public:

    auto build() const
        -> RaveAssetCollection;

};

inline auto createSampleAssets()
    -> RaveAssetCollection
{
    auto const builder = RaveAssetBuilder{};

    return builder.build();
}

} // namespace rave
