#include <Ape/CameraSelector.hpp>

#include <Ape/Scene.hpp>

#include <algorithm>

namespace ape
{

namespace
{

auto tryGetFirstCameraIndex(Scene const & scene)
    -> std::optional<int>
{
    if (scene.cameras.empty())
    {
        return std::nullopt;
    }

    return 0;
}

auto tryGetLastCameraIndex(Scene const & scene)
    -> std::optional<int>
{
    if (scene.cameras.empty())
    {
        return std::nullopt;
    }

    return static_cast<int>(scene.cameras.size() - 1);
}

auto getIndexInScene(Camera const & camera, Scene const & scene)
    -> std::optional<int>
{
    auto const it = std::find_if(
        std::cbegin(scene.cameras),
        std::cend(scene.cameras),
        [&camera] (Camera const & c)
    {
        return (&c == &camera);
    });

    if (it == std::cend(scene.cameras))
    {
        return std::nullopt;
    }

    return static_cast<int>(std::distance(std::cbegin(scene.cameras), it));
}

} // unnamed namespace

CameraSelector::CameraSelector(Scene & scene)
    : scene{&scene}
    , activeCameraIndex{tryGetFirstCameraIndex(scene)}
{
}

auto CameraSelector::getScene() const
    -> Scene &
{
    return *scene;
}

auto CameraSelector::getActiveCamera() const
    -> Camera *
{
    if (!activeCameraIndex)
    {
        return nullptr;
    }

    return &(scene->cameras[*activeCameraIndex]);
}

auto CameraSelector::activateCamera(int const index)
    -> void
{
    if ((index < 0) || (index >= static_cast<int>(scene->cameras.size())))
    {
        throw BadCameraIndex{index};
    }

    activeCameraIndex = index;

    onActiveCameraChanged.fire(getActiveCamera());
}

auto CameraSelector::activateNextCamera()
    -> void
{
    if (!activeCameraIndex)
    {
        activeCameraIndex = tryGetFirstCameraIndex(*scene);
    }
    else
    {
        auto const numOfCameras = static_cast<int>(scene->cameras.size());

        activeCameraIndex = (*activeCameraIndex + 1) % numOfCameras;
    }

    onActiveCameraChanged.fire(getActiveCamera());
}

auto CameraSelector::activatePreviousCamera()
    -> void
{
    if (!activeCameraIndex)
    {
        activeCameraIndex = tryGetLastCameraIndex(*scene);
    }
    else
    {
        auto const numOfCameras = static_cast<int>(scene->cameras.size());

        activeCameraIndex = (*activeCameraIndex - 1) % numOfCameras;
    }

    onActiveCameraChanged.fire(getActiveCamera());
}

auto CameraSelector::reset()
    -> void
{
    activeCameraIndex = std::nullopt;

    onActiveCameraChanged.fire(nullptr);
}

auto activateCamera(CameraSelector & selector, Camera & newCamera)
    -> void
{
    auto & scene = selector.getScene();

    auto const indexInScene = getIndexInScene(newCamera, scene);

    if (!indexInScene)
    {
        throw CameraNotInScene{};
    }

    selector.activateCamera(*indexInScene);
}

} // namespace ape
