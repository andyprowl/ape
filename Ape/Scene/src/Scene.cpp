#include <Ape/Scene/Scene.hpp>

namespace ape
{

Scene::Scene(std::vector<Body> bodies, std::vector<Camera> cameras, LightSystem lightSystem)
    : bodies{std::move(bodies)}
    , cameras{std::move(cameras)}
    , lightSystem{std::move(lightSystem)}
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

auto Scene::getLighting()
    -> LightSystem &
{
    return lightSystem;
}

auto Scene::getLighting() const
    -> LightSystem const &
{
    return lightSystem;
}

} // namespace ape
