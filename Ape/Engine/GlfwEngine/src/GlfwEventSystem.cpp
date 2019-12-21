#include <Ape/Engine/GlfwEngine/GlfwEventSystem.hpp>

#include "Glfw.hpp"

namespace ape
{

// virtual (from EventSystem)
auto GlfwEventSystem::pollEvents()
    -> void
{
    glfwPollEvents();
}

} // namespace ape
