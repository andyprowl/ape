#include <Ape/World/Scene/Camera.hpp>

#include <glm/gtc/matrix_transform.hpp>

namespace ape
{

Camera::Camera(
    CameraView::System const & system,
    OrthographicProjection::Frustum const & frustum)
    : Camera{CameraView{system, *this}, CameraProjection{frustum, *this}}
{
}

Camera::Camera(
    CameraView::System const & system,
    PerspectiveProjection::Frustum const & frustum)
    : Camera{CameraView{system, *this}, CameraProjection{frustum, *this}}
{
}

Camera::Camera(Camera && rhs) noexcept
    : view{std::move(rhs.view)}
    , projection{std::move(rhs.projection)}
    , transformation{rhs.transformation}
{
    setAsParentOfComponents();
}

auto Camera::operator = (Camera && rhs) noexcept
    -> Camera &
{
    view = std::move(rhs.view);
    
    projection = std::move(rhs.projection);
    
    transformation = rhs.transformation;

    setAsParentOfComponents();

    return *this;
}

auto Camera::getView()
    -> CameraView &
{
    return view;
}

auto Camera::getView() const
    -> CameraView const &
{
    return view;
}

auto Camera::setView(CameraView::System const & system)
    -> void
{
    view = CameraView{system, *this};

    updateTransformationAfterViewChanged();
}

auto Camera::getProjection()
    -> CameraProjection &
{
    return projection;
}

auto Camera::getProjection() const
    -> CameraProjection const &
{
    return projection;
}

auto Camera::setProjection(OrthographicProjection::Frustum const & frustum)
    -> void
{
    projection = frustum;

    updateTransformationAfterProjectionChanged();
}

auto Camera::setProjection(PerspectiveProjection::Frustum const & frustum)
    -> void
{
    projection = frustum;

    updateTransformationAfterProjectionChanged();
}

auto Camera::getTransformation() const
    -> glm::mat4 const &
{
    return transformation;
}

Camera::Camera(CameraView const & view, CameraProjection const & projection)
    : view{view}
    , projection{projection}
    , transformation{projection.getTransformation() * view.getTransformation()}
{
}

auto Camera::updateTransformationAfterViewChanged()
    -> void
{
    auto const & viewTransformation = view.getTransformation();

    updateTransformationAfterViewChanged(viewTransformation);
}

auto Camera::updateTransformationAfterViewChanged(glm::mat4 const & newView)
    -> void
{
    transformation = projection.getTransformation() * newView;

    onViewChanged.fire(newView);

    onTransformationChanged.fire(transformation);
}

auto Camera::updateTransformationAfterProjectionChanged()
    -> void
{
    auto const & projectionTransformation = projection.getTransformation();

    updateTransformationAfterProjectionChanged(projectionTransformation);
}

auto Camera::updateTransformationAfterProjectionChanged(glm::mat4 const & newProjection)
    -> void
{
    transformation = newProjection * view.getTransformation();

    onProjectionChanged.fire(newProjection);

    onTransformationChanged.fire(transformation);
}

auto Camera::setAsParentOfComponents()
    -> void
{
    view.setParent(*this);

    projection.setParent(*this);
}

} // namespace ape
