#pragma once

#include <Ape/Rendering/MonoDepthBodyRenderer.hpp>
#include <Ape/Rendering/OmniDepthBodyRenderer.hpp>

namespace ape
{

class DepthBodyRenderer
{

public:

    using BodySetView = ContainerView<std::vector<Body>>;

public:

    DepthBodyRenderer(MonoDepthBodyRenderer monoRenderer, OmniDepthBodyRenderer omniRenderer);

    auto render(
        BodySetView const & bodies,
        LightSystemView const & lightSystemView,
        DepthMapping & target) const
        -> void;

private:

    MonoDepthBodyRenderer monoRenderer;
    
    OmniDepthBodyRenderer omniRenderer;

};

} // namespace ape
