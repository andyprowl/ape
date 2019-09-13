#pragma once

class Position
{

public:

    Position(double const x, double const y)
        : x{x}
        , y{y}
    {
    }

public:

    double x;

    double y;

};

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
