#pragma once

#include "Body.hpp"
#include "Camera.hpp"
#include "DirectionalLight.hpp"
#include "PointLight.hpp"
#include "SpotLight.hpp"

#include <vector>

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

class World
{

public:

    World(std::vector<Body> bodies, Lighting lighting, Camera const & camera)
        : bodies{std::move(bodies)}
        , lighting{std::move(lighting)}
        , camera{camera}
    {
    }

public:

    std::vector<Body> bodies;

    Lighting lighting;

    Camera camera;

};