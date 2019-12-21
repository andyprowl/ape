#pragma once

#include <Ape/Engine/Engine/EventSystem.hpp>

namespace ape
{

class GlfwEventSystem : public EventSystem
{

public:

    // virtual (from EventSystem)
    auto pollEvents()
        -> void override;

};

} // namespace ape
