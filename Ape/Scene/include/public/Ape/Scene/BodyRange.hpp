#pragma once

#include <Range/ContainerView.hpp>

#include <vector>

namespace ape
{

class Body;

class BodyRange : public ContainerView<std::vector<Body *> const>
{

public:

    using ContainerView::ContainerView;

};

} // namespace ape
