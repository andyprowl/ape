#pragma once

namespace ape
{

class EventSystem
{

public:

    virtual ~EventSystem() = default;

    virtual auto pollEvents()
        -> void = 0;

};

} // namespace ape
