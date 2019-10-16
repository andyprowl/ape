#pragma once

#include <Rendering/LineStyle.hpp>

#include <Signal/Signal.hpp>

namespace ape
{

class LineStyleProvider
{

public:

    explicit LineStyleProvider(LineStyle const & style);

    auto getStyle() const
        -> LineStyle const &;

    auto setStyle(LineStyle const & newStyle)
        -> void;

public:

    mutable Signal<auto (LineStyle const &) -> void> onStyleChanged;

private:

    LineStyle style;

};

} // namespace ape
