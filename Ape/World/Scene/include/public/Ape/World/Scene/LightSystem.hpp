#pragma once

#include <Ape/World/Scene/DirectionalLight.hpp>
#include <Ape/World/Scene/PointLight.hpp>
#include <Ape/World/Scene/SpotLight.hpp>

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
