#include <Core/CameraSelector.hpp>

#include <Core/ContainerAlgorithms.hpp>
#include <Core/Scene.hpp>

#include <algorithm>
#include <numeric>

namespace ape
{

namespace
{

auto getAddressOfFirstCamera(Scene & scene)
    -> Camera const *
{
    auto const & cameras = scene.getCameras();

    if (cameras.empty())
    {
        return nullptr;
    }

    return &cameras.front();
}

auto asMutableCameraFromScene(Camera const & camera, Scene & scene)
    -> Camera &
{
    auto const & cameras = scene.getCameras();

    if (cameras.empty())
    {
        throw CameraNotInScene{};
    }

    if ((&camera < &cameras.front()) || (&camera > &cameras.back()))
    {
        throw CameraNotInScene{};
    }

    auto const index = static_cast<int>(std::distance(&cameras.front(), &camera));

    return scene.getCamera(index);
}

auto getAllMutableCamerasFromScene(Scene & scene)
    -> std::vector<Camera *>
{
    auto const & scenecameras = scene.getCameras();

    return transform(scenecameras, [&scene] (Camera const & camera)
    {
        return &asMutableCameraFromScene(camera, scene);
    });
}

} // unnamed namespace

CameraSelector::CameraSelector(Scene & scene)
    : scene{&scene}
    , firstCameraInScene{getAddressOfFirstCamera(scene)}
    , availableCameras{getAllMutableCamerasFromScene(scene)}
    , activeCameraIndex{tryGetFirstCameraIndex()}
    , cameraReallocationHandlerConnection{registerCamerReallocationHandler()}
{
}

auto CameraSelector::getScene() const
    -> Scene &
{
    return *scene;
}

auto CameraSelector::getAvailableCameras() const
    -> std::vector<Camera *> const &
{
    return availableCameras;
}

auto CameraSelector::getActiveCamera() const
    -> Camera *
{
    if (!activeCameraIndex)
    {
        return nullptr;
    }

    return availableCameras[*activeCameraIndex];
}

auto CameraSelector::activateCamera(int const index)
    -> void
{
    if (index >= static_cast<int>(availableCameras.size()))
    {
        throw BadCameraIndex{index};
    }

    activeCameraIndex = index;

    onActiveCameraChanged.fire(getActiveCamera());
}

auto CameraSelector::activateCamera(Camera const & camera)
    -> void
{
    if (getActiveCamera() == &camera)
    {
        return;
    }

    auto & mutableCamera = asMutableCameraFromScene(camera, *scene);

    auto const it = find(availableCameras, &mutableCamera);

    if (it == std::cend(availableCameras))
    {
        availableCameras.push_back(&mutableCamera);
    }

    activeCameraIndex = static_cast<int>(std::distance(std::cbegin(availableCameras), it));

    onActiveCameraChanged.fire(getActiveCamera());
}

auto CameraSelector::activateNextCamera()
    -> void
{
    if (!activeCameraIndex)
    {
        activeCameraIndex = tryGetFirstCameraIndex();
    }
    else
    {
        auto const numOfCameras = static_cast<int>(availableCameras.size());

        activeCameraIndex = (*activeCameraIndex + 1) % numOfCameras;
    }

    onActiveCameraChanged.fire(getActiveCamera());
}

auto CameraSelector::activatePreviousCamera()
    -> void
{
    if (!activeCameraIndex)
    {
        activeCameraIndex = tryGetLastCameraIndex();
    }
    else
    {
        auto const numOfCameras = static_cast<int>(availableCameras.size());

        activeCameraIndex = (*activeCameraIndex + numOfCameras - 1) % numOfCameras;
    }

    onActiveCameraChanged.fire(getActiveCamera());
}

auto CameraSelector::deactivateCamera()
    -> void
{
    activeCameraIndex = std::nullopt;

    onActiveCameraChanged.fire(nullptr);
}

auto CameraSelector::tryGetFirstCameraIndex() const
    -> std::optional<int>
{
    if (availableCameras.empty())
    {
        return std::nullopt;
    }

    return 0;
}

auto CameraSelector::tryGetLastCameraIndex() const
    -> std::optional<int>
{
    if (availableCameras.empty())
    {
        return std::nullopt;
    }

    return static_cast<int>(availableCameras.size() - 1);
}

auto CameraSelector::registerCamerReallocationHandler()
    -> ScopedSignalConnection
{
    return scene->onCameraReallocation.registerHandler([this]
    {
        restoreValidCameraReferences(availableCameras);

        firstCameraInScene = getAddressOfFirstCamera(*scene);
    });
}

auto CameraSelector::restoreValidCameraReferences(std::vector<Camera *> & cameras) const
    -> void
{
    std::transform(
        std::begin(cameras),
        std::end(cameras),
        std::begin(cameras),
        [this] (Camera const * const camera)
    {
        auto const index = getCameraIndexInOriginalContainer(*camera);

        return &(scene->getCamera(index));
    });
}

auto CameraSelector::getCameraIndexInOriginalContainer(Camera const & camera) const
    -> int
{
    return static_cast<int>(std::distance(firstCameraInScene, &camera));
}

} // namespace ape
