#include <Scene/Scene.hpp>

namespace ape
{

Scene::Scene(std::vector<Body> bodies, std::vector<Camera> cameras, Lighting lighting)
    : bodies{std::move(bodies)}
    , cameras{std::move(cameras)}
    , lighting{std::move(lighting)}
{
}

auto Scene::getBody(int const index)
    -> Body &
{
    return bodies[index];
}


auto Scene::getBodies()
    -> ContainerView<std::vector<Body>>
{
    return ContainerView<std::vector<Body>>{bodies};
}

auto Scene::getBodies() const
    -> ContainerView<std::vector<Body> const>
{
    return ContainerView<std::vector<Body> const>{bodies};
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
    -> ContainerView<std::vector<Camera>>
{
    return ContainerView<std::vector<Camera>>{cameras};
}

auto Scene::getCameras() const
    -> ContainerView<std::vector<Camera> const>
{
    return ContainerView<std::vector<Camera> const>{cameras};
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
    -> Lighting &
{
    return lighting;
}

auto Scene::getLighting() const
    -> Lighting const &
{
    return lighting;
}

} // namespace ape
