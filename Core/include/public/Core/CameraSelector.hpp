#pragma once

#include <Core/Signal.hpp>

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
        : logic_error{"Index " + std::to_string(index) + " is not a valid camera index"}
    {
    }

};

class CameraSelector
{

public:

    explicit CameraSelector(Scene & scene);

    CameraSelector(Scene & scene, std::vector<int> availableCameraIndices);

    auto getScene() const
        -> Scene &;
    
    auto getActiveCamera() const
        -> Camera *;

    auto activateCamera(int index)
        -> void;

    auto activateNextCamera()
        -> void;

    auto activatePreviousCamera()
        -> void;

    auto reset()
        -> void;

public:

    mutable Signal<auto (Camera *) -> void> onActiveCameraChanged;

private:

    auto tryGetFirstCameraIndex() const
        -> std::optional<int>;

    auto tryGetLastCameraIndex() const
        -> std::optional<int>;

private:

    Scene * scene;

    // These are indices into the scene's camera containers.
    std::vector<int> availableCameraIndices;

    // This is an index into the "availableCameraIndices" data member above.
    std::optional<int> activeCameraIndex;

};

auto activateCamera(CameraSelector & selector, Camera & newCamera)
    -> void;

} // namespace ape
