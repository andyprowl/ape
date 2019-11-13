#pragma once

#include <Rave/RaveCore/RaveAssetCollection.hpp>

namespace rave
{

class RaveAssetBuilder
{

public:

    RaveAssetBuilder();

    explicit RaveAssetBuilder(bool doNotIncludeSponza);

    auto build() const
        -> RaveAssetCollection;

private:

    bool doNotIncludeSponza;

};

inline auto createRaveAssets(bool const doNotIncludeSponza)
    -> RaveAssetCollection
{
    auto const builder = RaveAssetBuilder{doNotIncludeSponza};

    return builder.build();
}

} // namespace rave
