#include <Initialization/Initialization.hpp>

#include <Initialization/OpenGLLoader.hpp>

namespace ape
{

auto loader = OpenGLLoader{false, true};

auto initialize()
    -> void
{
    loader.loadFunctions();
}

} // namespace ape
