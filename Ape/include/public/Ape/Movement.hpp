#pragma once

namespace ape
{

class Movement
{

public:

    Movement(double const deltaX, double const deltaY)
        : deltaX{deltaX}
        , deltaY{deltaY}
    {
    }

public:

    double deltaX;

    double deltaY;

};

} // namespace ape
