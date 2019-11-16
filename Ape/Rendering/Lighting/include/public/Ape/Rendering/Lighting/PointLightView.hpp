#pragma once

#include <Ape/Rendering/Lighting/PointLightViewTransformation.hpp>

#include <Ape/World/Scene/Camera.hpp>

#include <Basix/Mathematics/Size.hpp>
#include <Basix/Signal/ScopedSignalConnection.hpp>

namespace ape
{

class PointLight;

class PointLightView
{

public:

    class FaceCameraSet
    {

    public:

        FaceCameraSet(
            Camera right,
            Camera left,
            Camera top,
            Camera bottom,
            Camera front,
            Camera back)
            : right{std::move(right)}
            , left{std::move(left)}
            , top{std::move(top)}
            , bottom{std::move(bottom)}
            , front{std::move(front)}
            , back{std::move(back)}
        {
        }

    public:

        Camera right;

        Camera left;

        Camera top;

        Camera bottom;

        Camera front;

        Camera back;

    };

public:

    explicit PointLightView(PointLight const & light);

    PointLightView(PointLightView const & rhs) = delete;

    PointLightView(PointLightView && rhs) noexcept;

    auto operator = (PointLightView const & rhs)
        -> PointLightView & = delete;

    auto operator = (PointLightView && rhs) noexcept
        -> PointLightView &;

    ~PointLightView() = default;
    
    auto getFaceCameras() const
        -> const FaceCameraSet &;

private:

    auto makeLightFaceCameras() const
        -> FaceCameraSet;

    auto updateLightFaceCameras()
        -> void;

    auto registerForLightChangeNotifications()
        -> basix::ScopedSignalConnection;

private:

    PointLight const * light;

    FaceCameraSet faceCameras;

    basix::ScopedSignalConnection onLightChangedConnection;

};

inline auto getTransformation(PointLightView const & view)
    -> PointLightViewTransformation
{
    auto const & cameras = view.getFaceCameras();

    return {
        cameras.right.getTransformation(),
        cameras.left.getTransformation(),
        cameras.top.getTransformation(),
        cameras.bottom.getTransformation(),
        cameras.front.getTransformation(),
        cameras.back.getTransformation()};
}

} // namespace ape
