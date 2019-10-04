#pragma once

namespace ape
{

class Shape;

class ShapeRenderer
{

public:

    virtual ~ShapeRenderer() = default;

    virtual auto render(Shape const & shape) const
        -> void = 0;

};

} // namespace ape
