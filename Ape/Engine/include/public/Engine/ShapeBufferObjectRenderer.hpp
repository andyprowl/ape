#pragma once

#include <Engine/ShapeRenderer.hpp>

namespace ape
{

class Shape;

class ShapeBufferObjectRenderer : public ShapeRenderer
{

public:

    // virtual (from ShapeRenderer)
    auto render(Shape const & shape) const
        -> void override;

};

} // namespace ape
