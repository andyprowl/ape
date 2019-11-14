#pragma once

#include <Basix/Range/ContainerView.hpp>

#include <vector>

namespace ape
{

class Body;

class BodyRange : public basix::ContainerView<std::vector<Body *> const>
{

public:

    using ContainerView::ContainerView;

};

} // namespace ape
