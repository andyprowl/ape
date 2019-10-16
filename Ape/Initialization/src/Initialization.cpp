#include <Ape/Initialization/Initialization.hpp>

#include <Ape/Initialization/OpenGLLoader.hpp>

namespace ape
{

auto loader = OpenGLLoader{false, true};

auto initialize()
    -> void
{
    loader.loadFunctions();
}

} // namespace ape
