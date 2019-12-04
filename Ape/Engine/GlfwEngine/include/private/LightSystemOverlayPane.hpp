#pragma once

#include <Ape/World/Scene/Light.hpp>

#include <glm/vec3.hpp>

#include <string_view>
#include <type_traits>

namespace ape
{

class LightSystem;

template<typename LightType>
class LightSystemOverlayPane
{

public:

    static_assert(std::is_base_of_v<Light, LightType>, "LightType must derive from Light!");

public:

    explicit LightSystemOverlayPane(std::vector<LightType> & lights);

    auto update()
        -> void;

    auto getSelectedLight() const
        -> LightType *;

private:

    auto updateTogglingPanel()
        -> void;

    auto updateDetailPanel()
        -> void;

    auto updateLightSelector()
        -> void;

    virtual auto updateSelectedLightProperties()
        -> void = 0;

private:

    std::vector<LightType> * lights;

    LightType * selectedLight;

};

} // namespace ape
