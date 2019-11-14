#pragma once

#include <Basix/Signal/Signal.hpp>

#include <glm/mat4x4.hpp>

namespace ape
{

class Camera;

class PerspectiveProjection
{

public:

    class Frustum
    {

    public:

        Frustum(
            float const fieldOfView,
            float const aspectRatio,
            float const near,
            float const far)
            : fieldOfView{fieldOfView}
            , aspectRatio{aspectRatio}
            , near{near}
            , far{far}
        {
        }
    
    public:
    
        float fieldOfView;

        float aspectRatio;

        float near;

        float far;

    };

public:

    PerspectiveProjection(Frustum const & frustum, Camera & parent);

    auto getTransformation() const
        -> glm::mat4 const &;

    auto getFrustum() const
        -> Frustum const &;

    auto setFrustum(Frustum const & newFrustum)
        -> void;

    auto getFieldOfView() const
        -> float;

    auto setFieldOfView(float newFieldOfView)
        -> void;

    auto getAspectRatio() const
        -> float;

    auto setAspectRatio(float newAspectRatio)
        -> void;

    auto getParent() const
        -> Camera &;

private:

    friend class CameraProjection;

private:

    auto makeProjection() const
        -> glm::mat4;

    auto recalculateProjectionAndNotifyParent()
        -> void;

    auto setParent(Camera & camera)
        -> void;

private:

    Frustum frustum;

    glm::mat4 transformation;

    Camera * parent;

};

} // namespace ape
