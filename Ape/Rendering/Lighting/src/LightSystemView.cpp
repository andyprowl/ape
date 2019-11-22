#include <Ape/Rendering/Lighting/LightSystemView.hpp>

#include <Ape/World/Scene/Camera.hpp>
#include <Ape/World/Scene/LightSystem.hpp>

#include <Basix/Range/Transform.hpp>

namespace ape
{

namespace
{

auto makePointLightView(LightSystem const & lightSystem)
    -> std::vector<PointLightView>
{
    return basix::transform(lightSystem.point, [] (PointLight const & light)
    {
        return PointLightView{light};
    });
}

auto makeSpotLightView(LightSystem const & lightSystem, basix::Size<int> const & viewSize)
    -> std::vector<SpotLightView>
{
    return basix::transform(lightSystem.spot, [&viewSize] (SpotLight const & light)
    {
        return SpotLightView{light, viewSize};
    });
}

auto makeDirectionalLightView(LightSystem const & lightSystem)
    -> std::vector<DirectionalLightView>
{
    return basix::transform(lightSystem.directional, [] (DirectionalLight const & light)
    {
        return DirectionalLightView{light};
    });
}

} // unnamed namespace

LightSystemView::LightSystemView(
    LightSystem const & lightSystem,
    basix::Size<int> const & viewSize)
    : lightSystem{&lightSystem}
    , viewSize{viewSize}
    , pointView{makePointLightView(lightSystem)}
    , spotView{makeSpotLightView(lightSystem, viewSize)}
    , directionalView{makeDirectionalLightView(lightSystem)}
{
}

auto LightSystemView::getLighting() const
    -> LightSystem const &
{
    return *lightSystem;
}

auto LightSystemView::getPointView() const
    -> std::vector<PointLightView> const &
{
    return pointView;
}

auto LightSystemView::getSpotView() const
    -> std::vector<SpotLightView> const &
{
    return spotView;
}

auto LightSystemView::getDirectionalView() const
    -> std::vector<DirectionalLightView> const &
{
    return directionalView;
}

auto LightSystemView::setViewSize(basix::Size<int> const & newViewSize)
    -> void
{
    viewSize = newViewSize;

    for (auto & view : spotView)
    {
        view.setViewSize(newViewSize);
    }
}

} // namespace ape
