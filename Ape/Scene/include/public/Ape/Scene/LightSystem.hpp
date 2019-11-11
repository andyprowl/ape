#pragma once

#include <Ape/Scene/DirectionalLight.hpp>
#include <Ape/Scene/PointLight.hpp>
#include <Ape/Scene/SpotLight.hpp>

#include <vector>

namespace ape
{

class LightSystem
{

public:

    LightSystem() = default;

    LightSystem(
        std::vector<PointLight> point,
        std::vector<SpotLight> spot,
        std::vector<DirectionalLight> directional)
        : point{std::move(point)}
        , spot{std::move(spot)}
        , directional{std::move(directional)}
    {
    }

public:

    std::vector<PointLight> point;

    std::vector<SpotLight> spot;

    std::vector<DirectionalLight> directional;

};

} // namespace ape
