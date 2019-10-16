#pragma once

#include <Signal/Signal.hpp>

#include <glm/mat4x4.hpp>

namespace ape
{

class Camera;

class OrthographicProjection
{

public:

    class Frustum
    {

    public:

        Frustum(
            float const left,
            float const right,
            float const bottom,
            float const top,
            float const near,
            float const far)
            : left{left}
            , right{right}
            , bottom{bottom}
            , top{top}
            , near{near}
            , far{far}
        {
        }
    
    public:
    
        float left;

        float right;

        float bottom;

        float top;

        float near;

        float far;

    };

public:

    OrthographicProjection(Frustum const & frustum, Camera & parent);

    auto getTransformation() const
        -> glm::mat4 const &;

    auto getFrustum() const
        -> Frustum const &;

    auto setFrustum(Frustum const & newFrustum)
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
