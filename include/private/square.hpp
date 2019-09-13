#pragma once

#include "shape.hpp"

enum class SquareNormalDirection
{
    inbound,
    outbound
};

auto makeSquare(SquareNormalDirection normalDirection)
    -> Shape;
