#pragma once

namespace ape
{

class Shape;

class ShapeDrawer
{

public:

    virtual ~ShapeDrawer() = default;

    virtual auto beginDrawBatch()
        -> void = 0;

    virtual auto draw(Shape const & shape)
        -> void = 0;

    virtual auto endDrawBatch()
        -> void = 0;

};

} // namespace ape
