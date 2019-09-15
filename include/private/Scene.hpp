#pragma once

#include "Mesh.hpp"
#include "Camera.hpp"
#include "Lighting.hpp"

#include <vector>

class Scene
{

public:

    Scene(std::vector<Mesh> bodies, Lighting lighting, Camera const & camera)
        : bodies{std::move(bodies)}
        , lighting{std::move(lighting)}
        , camera{camera}
    {
    }

public:

    std::vector<Mesh> bodies;

    Lighting lighting;

    Camera camera;

};