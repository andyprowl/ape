#include <Ape/Rendering/LineStyleProvider.hpp>

namespace ape
{

LineStyleProvider::LineStyleProvider(LineStyle const & style)
    : style{style}
{
}

auto LineStyleProvider::getStyle() const
    -> LineStyle const &
{
    return style;
}

auto LineStyleProvider::setStyle(LineStyle const & newStyle)
    -> void
{
    style = newStyle;

    onStyleChanged.fire(style);
}

} // namespace ape
