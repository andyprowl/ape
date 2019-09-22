#pragma once

#include <Ape/DirectionalLight.hpp>
#include <Ape/PointLight.hpp>
#include <Ape/SpotLight.hpp>

#include <vector>

namespace ape
{

class Lighting
{

public:

    Lighting() = default;

    Lighting(
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
