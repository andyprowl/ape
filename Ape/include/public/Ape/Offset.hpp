#pragma once

namespace ape
{

class Offset
{

public:

    Offset(double const deltaX, double const deltaY)
        : deltaX{deltaX}
        , deltaY{deltaY}
    {
    }

public:

    double deltaX;

    double deltaY;

};

} // namespace ape
