#include <Ape/World/Scene/Scene.hpp>

namespace ape
{

Scene::Scene(
    std::vector<Body> bodies,
    std::vector<Camera> cameras,
    LightSystem lightSystem,
    Fog const & fog)
    : bodies{std::move(bodies)}
    , cameras{std::move(cameras)}
    , lightSystem{std::move(lightSystem)}
    , fog{fog}
{
}

auto Scene::getBody(int const index)
    -> Body &
{
    return bodies[index];
}

auto Scene::getBodies()
    -> basix::ContainerView<std::vector<Body>>
{
    return basix::ContainerView<std::vector<Body>>{bodies};
}

auto Scene::getBodies() const
    -> basix::ContainerView<std::vector<Body> const>
{
    return basix::ContainerView<std::vector<Body> const>{bodies};
}

auto Scene::getNumOfBodies() const
    -> int
{
    return static_cast<int>(bodies.size());
}

auto Scene::addBody(Body body)
    -> Body &
{
    auto const isReallocationRequired = (bodies.size() == bodies.capacity());

    auto & addedBody = bodies.emplace_back(std::move(body));

    onBodyAdded.fire(addedBody);

    if (isReallocationRequired)
    {
        onBodyReallocation.fire();
    }

    return addedBody;
}

auto Scene::reserveBodyCapacity(const int capacity)
    -> void
{
    return bodies.reserve(static_cast<std::size_t>(capacity));
}

auto Scene::getCamera(int const index)
    -> Camera &
{
    return cameras[index];
}

auto Scene::getCameras()
    -> basix::ContainerView<std::vector<Camera>>
{
    return basix::ContainerView<std::vector<Camera>>{cameras};
}

auto Scene::getCameras() const
    -> basix::ContainerView<std::vector<Camera> const>
{
    return basix::ContainerView<std::vector<Camera> const>{cameras};
}

auto Scene::getNumOfCameras() const
    -> int
{
    return static_cast<int>(cameras.size());
}

auto Scene::addCamera(Camera camera)
    -> Camera &
{
    auto const isReallocationRequired = (cameras.size() == cameras.capacity());

    auto & addedCamera = cameras.emplace_back(std::move(camera));

    onCameraAdded.fire(addedCamera);

    if (isReallocationRequired)
    {
        onCameraReallocation.fire();
    }

    return addedCamera;
}

auto Scene::getLightSystem()
    -> LightSystem &
{
    return lightSystem;
}

auto Scene::getLightSystem() const
    -> LightSystem const &
{
    return lightSystem;
}

auto Scene::getFog()
    -> Fog &
{
    return fog;
}

auto Scene::getFog() const
    -> Fog const &
{
    return fog;
}

} // namespace ape
