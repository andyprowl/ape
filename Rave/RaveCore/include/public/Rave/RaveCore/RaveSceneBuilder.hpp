#pragma once

#include <Rave/RaveCore/RaveScene.hpp>

namespace rave
{

class RaveAssetCollection;

class RaveSceneBuilder
{

public:

    explicit RaveSceneBuilder(RaveAssetCollection & assets);

    RaveSceneBuilder(RaveAssetCollection & assets, bool excludeSponza);

    auto build() const
        -> RaveScene;

private:

    RaveAssetCollection * assets;

    bool excludeSponza;

};

inline auto createRaveScene(RaveAssetCollection & assets, bool const excludeSponza)
    -> RaveScene
{
    auto const builder = RaveSceneBuilder{assets, excludeSponza};

    return builder.build();
}

} // namespace rave
