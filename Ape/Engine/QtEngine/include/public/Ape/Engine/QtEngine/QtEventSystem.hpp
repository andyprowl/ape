#pragma once

#include <Ape/Engine/Engine/EventSystem.hpp>

namespace ape::qt
{

class QtEventSystem : public EventSystem
{

public:

    // virtual (from EventSystem)
    auto pollEvents()
        -> void override
    {
        // Nothing to do here, events are polled by the Qt framework.
    }

};

} // namespace ape::qt
