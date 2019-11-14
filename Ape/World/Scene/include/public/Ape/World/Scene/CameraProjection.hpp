#pragma once

#include <Ape/World/Scene/OrthographicProjection.hpp>
#include <Ape/World/Scene/PerspectiveProjection.hpp>

#include <variant>

namespace ape
{

class Camera;

class CameraProjection
{

public:

    CameraProjection(OrthographicProjection::Frustum const & orthographicFrustum, Camera & parent);

    CameraProjection(PerspectiveProjection::Frustum const & perspectiveFrustum, Camera & parent);

    CameraProjection(CameraProjection const & rhs) = default;

    CameraProjection(CameraProjection && rhs) noexcept = default;

    auto operator = (CameraProjection const & rhs)
        -> CameraProjection & = default;

    auto operator = (CameraProjection && rhs) noexcept
        -> CameraProjection & = default;

    auto operator = (OrthographicProjection::Frustum const & orthographicFrustum)
        -> CameraProjection &;

    auto operator = (PerspectiveProjection::Frustum const & orthographicFrustum)
        -> CameraProjection &;

    ~CameraProjection() = default;

    auto getTransformation() const
        -> glm::mat4 const &;

    template<typename ProjectionType>
    auto is() const
        -> bool
    {
        return (std::get_if<ProjectionType>(&projection) != nullptr);
    }

    template<typename ProjectionType>
    auto as()
        -> ProjectionType &
    {
        return std::get<ProjectionType>(projection);
    }

    template<typename ProjectionType>
    auto as() const
        -> ProjectionType const &
    {
        return std::get<ProjectionType>(projection);
    }

    template<typename ProjectionType>
    auto tryAs()
        -> ProjectionType *
    {
        return std::get_if<ProjectionType>(&projection);
    }

    template<typename ProjectionType>
    auto tryAs() const
        -> ProjectionType const *
    {
        return std::get_if<ProjectionType>(&projection);
    }

    auto getParent() const
        -> Camera &;

private:

    friend class Camera;

private:

    auto setParent(Camera & camera)
        -> void;

private:

    std::variant<OrthographicProjection, PerspectiveProjection> projection;

};

} // namespace ape
