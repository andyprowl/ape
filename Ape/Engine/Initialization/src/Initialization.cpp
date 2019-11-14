#include <Ape/Engine/Initialization/Initialization.hpp>

#include <Ape/Engine/Initialization/OpenGLLoader.hpp>

namespace ape
{

auto loader = OpenGLLoader{false, true};

auto initialize()
    -> void
{
    loader.loadFunctions();
}

} // namespace ape
