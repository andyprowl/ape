#include <Core/CameraSelector.hpp>

#include <Core/Scene.hpp>

#include <algorithm>
#include <numeric>

namespace ape
{

namespace
{

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

auto getAllAvailableCameraIndices(Scene const & scene)
    -> std::vector<int>
{
    auto indices = std::vector<int>{};

    indices.resize(scene.cameras.size());

    std::iota(std::begin(indices), std::end(indices), 0);

    return indices;
}

} // unnamed namespace

CameraSelector::CameraSelector(Scene & scene)
    : CameraSelector{scene, getAllAvailableCameraIndices(scene)}
{
}

CameraSelector::CameraSelector(Scene & scene, std::vector<int> availableCameraIndices)
    : scene{&scene}
    , availableCameraIndices{std::move(availableCameraIndices)}
    , activeCameraIndex{tryGetFirstCameraIndex()}
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

    auto const sceneCameraIndex = availableCameraIndices[*activeCameraIndex];

    return &(scene->cameras[sceneCameraIndex]);
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
        activeCameraIndex = tryGetFirstCameraIndex();
    }
    else
    {
        auto const numOfCameras = static_cast<int>(availableCameraIndices.size());

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
        auto const numOfCameras = static_cast<int>(availableCameraIndices.size());

        activeCameraIndex = (*activeCameraIndex + numOfCameras - 1) % numOfCameras;
    }

    onActiveCameraChanged.fire(getActiveCamera());
}

auto CameraSelector::reset()
    -> void
{
    activeCameraIndex = std::nullopt;

    onActiveCameraChanged.fire(nullptr);
}

auto CameraSelector::tryGetFirstCameraIndex() const
    -> std::optional<int>
{
    if (availableCameraIndices.empty())
    {
        return std::nullopt;
    }

    return 0;
}

auto CameraSelector::tryGetLastCameraIndex() const
    -> std::optional<int>
{
    if (availableCameraIndices.empty())
    {
        return std::nullopt;
    }

    return static_cast<int>(availableCameraIndices.size() - 1);
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
