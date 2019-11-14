#pragma once

#include <Basix/Signal/Signal.hpp>

#include <optional>
#include <stdexcept>
#include <string>

namespace ape
{

class Camera;
class Scene;

class CameraNotInScene : public std::logic_error
{

public:

    CameraNotInScene()
        : logic_error{"The specified camera does not belong to the attached scene"}
    {
    }

};

class BadCameraIndex : public std::logic_error
{

public:

    explicit BadCameraIndex(int index)
        : logic_error{"Index " + std::to_string(index) + " is not an available camera index"}
    {
    }

};

class CameraSelector
{

public:

    explicit CameraSelector(Scene & scene);

    auto getScene() const
        -> Scene &;

    auto getAvailableCameras() const
        -> std::vector<Camera *> const &;

    auto getActiveCamera() const
        -> Camera *;
    
    auto activateCamera(int index)
        -> void;

    auto activateCamera(Camera const & camera)
        -> void;

    auto activateNextCamera()
        -> void;

    auto activatePreviousCamera()
        -> void;

    auto deactivateCamera()
        -> void;

public:

    mutable basix::Signal<auto (Camera *) -> void> onActiveCameraChanged;

private:

    auto tryGetFirstCameraIndex() const
        -> std::optional<int>;

    auto tryGetLastCameraIndex() const
        -> std::optional<int>;

    auto registerCamerReallocationHandler()
        -> basix::ScopedSignalConnection;

    auto restoreValidCameraReferences(std::vector<Camera *> & cameras) const
        -> void;

    auto getCameraIndexInOriginalContainer(Camera const & camera) const
        -> int;

private:

    Scene * scene;

    Camera const * firstCameraInScene;

    std::vector<Camera *> availableCameras;

    std::optional<int> activeCameraIndex;

    basix::ScopedSignalConnection cameraReallocationHandlerConnection;

};

} // namespace ape
