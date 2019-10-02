#pragma once

namespace ape
{

class TaitBryanAngles
{

public:

    TaitBryanAngles(float const yaw, float const pitch, float const roll)
        : yaw{yaw}
        , pitch{pitch}
        , roll{roll}
    {
    }

public:

    float yaw;

    float pitch;

    float roll;

};

} // namespace ape
