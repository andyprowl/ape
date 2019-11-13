#pragma once

#include <Rave/RaveCore/RaveScene.hpp>

namespace rave
{

class RaveAssetCollection;

class RaveSceneBuilder
{

public:

    explicit RaveSceneBuilder(RaveAssetCollection & assets);

    RaveSceneBuilder(RaveAssetCollection & assets, bool doNotIncludeSponza);

    auto build() const
        -> RaveScene;

private:

    RaveAssetCollection * assets;

    bool doNotIncludeSponza;

};

inline auto createRaveScene(RaveAssetCollection & assets, bool const doNotIncludeSponza)
    -> RaveScene
{
    auto const builder = RaveSceneBuilder{assets, doNotIncludeSponza};

    return builder.build();
}

} // namespace rave
