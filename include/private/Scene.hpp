#pragma once

#include "Mesh.hpp"
#include "Camera.hpp"
#include "Lighting.hpp"

#include <vector>

class Scene
{

public:

    Scene(std::vector<Mesh> meshes, Lighting lighting, Camera const & camera)
        : meshes{std::move(meshes)}
        , lighting{std::move(lighting)}
        , camera{camera}
    {
    }

public:

    std::vector<Mesh> meshes;

    Lighting lighting;

    Camera camera;

};