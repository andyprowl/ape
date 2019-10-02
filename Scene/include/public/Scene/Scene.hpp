#pragma once

#include <Scene/Body.hpp>
#include <Scene/Camera.hpp>
#include <Scene/Lighting.hpp>

#include <Range/ContainerView.hpp>

#include <Signal/Signal.hpp>

#include <vector>

namespace ape
{

class Scene
{

public:

    Scene() = default;

    Scene(std::vector<Body> bodies, std::vector<Camera> cameras, Lighting lighting);

    auto getBody(int index)
        -> Body &;

    auto getBodies()
        -> ContainerView<std::vector<Body>>;

    auto getBodies() const
        -> ContainerView<std::vector<Body> const>;

    auto getNumOfBodies() const
        -> int;

    auto addBody(Body body)
        -> Body &;

    auto getCamera(int index)
        -> Camera &;

    auto getCameras()
        -> ContainerView<std::vector<Camera>>;

    auto getCameras() const
        -> ContainerView<std::vector<Camera> const>;

    auto getNumOfCameras() const
        -> int;

    auto addCamera(Camera camera)
        -> Camera &;

    auto getLighting()
        -> Lighting &;

    auto getLighting() const
        -> Lighting const &;

public:

    mutable Signal<auto (Body &) -> void> onBodyAdded;

    mutable Signal<auto () -> void> onBodyReallocation;

    mutable Signal<auto (Camera &) -> void> onCameraAdded;

    mutable Signal<auto () -> void> onCameraReallocation;

private:

    std::vector<Body> bodies;

    std::vector<Camera> cameras;

    Lighting lighting;

};

} // namespace ape
