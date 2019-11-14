#pragma once

#include <Ape/World/Scene/Body.hpp>
#include <Ape/World/Scene/Camera.hpp>
#include <Ape/World/Scene/LightSystem.hpp>

#include <Basix/Range/ContainerView.hpp>
#include <Basix/Signal/Signal.hpp>

#include <vector>

namespace ape
{

class Scene
{

public:

    Scene() = default;

    Scene(std::vector<Body> bodies, std::vector<Camera> cameras, LightSystem lightSystem);

    auto getBody(int index)
        -> Body &;

    auto getBodies()
        -> basix::ContainerView<std::vector<Body>>;

    auto getBodies() const
        -> basix::ContainerView<std::vector<Body> const>;

    auto getNumOfBodies() const
        -> int;

    auto addBody(Body body)
        -> Body &;

    auto getCamera(int index)
        -> Camera &;

    auto getCameras()
        -> basix::ContainerView<std::vector<Camera>>;

    auto getCameras() const
        -> basix::ContainerView<std::vector<Camera> const>;

    auto getNumOfCameras() const
        -> int;

    auto addCamera(Camera camera)
        -> Camera &;

    auto getLighting()
        -> LightSystem &;

    auto getLighting() const
        -> LightSystem const &;

public:

    mutable basix::Signal<auto (Body &) -> void> onBodyAdded;

    mutable basix::Signal<auto () -> void> onBodyReallocation;

    mutable basix::Signal<auto (Camera &) -> void> onCameraAdded;

    mutable basix::Signal<auto () -> void> onCameraReallocation;

private:

    std::vector<Body> bodies;

    std::vector<Camera> cameras;

    LightSystem lightSystem;

};

} // namespace ape
