#pragma once

#include <Ape/World/Scene/CameraProjection.hpp>
#include <Ape/World/Scene/CameraView.hpp>

#include <Basix/Signal/Signal.hpp>

#include <glm/mat4x4.hpp>

namespace ape
{

class Camera
{

public:

    using TransformEventSignature = auto (glm::mat4 const & transformation) -> void;

public:

    Camera(
        CameraView::System const & system,
        OrthographicProjection::Frustum const & frustum);

    Camera(
        CameraView::System const & system,
        PerspectiveProjection::Frustum const & frustum);

    Camera(Camera const & rhs) = delete;

    Camera(Camera && rhs) noexcept;

    auto operator = (Camera const & rhs) noexcept
        -> Camera & = delete;

    auto operator = (Camera && rhs) noexcept
        -> Camera &;

    ~Camera() = default;

    auto getView()
        -> CameraView &;

    auto getView() const
        -> CameraView const &;

    auto setView(CameraView::System const & system)
        -> void;

    auto getProjection()
        -> CameraProjection &;

    auto getProjection() const
        -> CameraProjection const &;

    auto setProjection(OrthographicProjection::Frustum const & frustum)
        -> void;

    auto setProjection(PerspectiveProjection::Frustum const & frustum)
        -> void;

    auto getTransformation() const
        -> glm::mat4 const &;

public:

    mutable basix::Signal<TransformEventSignature> onViewChanged;

    mutable basix::Signal<TransformEventSignature> onProjectionChanged;

    mutable basix::Signal<TransformEventSignature> onTransformationChanged;

private:

    friend class CameraView;

    friend class OrthographicProjection;

    friend class PerspectiveProjection;

private:

    Camera(CameraView const & view, CameraProjection const & projection);

    auto updateTransformationAfterViewChanged()
        -> void;

    auto updateTransformationAfterViewChanged(glm::mat4 const & newView)
        -> void;

    auto updateTransformationAfterProjectionChanged()
        -> void;

    auto updateTransformationAfterProjectionChanged(glm::mat4 const & newProjection)
        -> void;

    auto setAsParentOfComponents()
        -> void;

private:

    CameraView view;

    CameraProjection projection;

    glm::mat4 transformation;

};

} // namespace ape
