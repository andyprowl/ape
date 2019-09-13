#pragma once

#include "Shape.hpp"

enum class SquareNormalDirection
{
    inbound,
    outbound
};

auto makeSquare(SquareNormalDirection normalDirection)
    -> Shape;
