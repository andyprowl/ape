#pragma once

#include <Application/RaveCore/RaveScene.hpp>

namespace rave
{

class RaveAssetCollection;

class RaveSceneBuilder
{

public:

    explicit RaveSceneBuilder(RaveAssetCollection & assets);

    auto build() const
        -> RaveScene;

private:

    RaveAssetCollection * assets;

};

inline auto createRaveScene(RaveAssetCollection & assets)
    -> RaveScene
{
    auto const builder = RaveSceneBuilder{assets};

    return builder.build();
}

} // namespace rave
