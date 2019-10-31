#include "Application.hpp"

#include <Foundational/Range/Search.hpp>

#include <iostream>
#include <exception>
#include <string>
#include <vector>

auto isDebugOutputEnabled(std::vector<std::string> const & arguments)
    -> bool
{
    return ape::contains(arguments, "--enable-debug-output");
}

auto main(int argc, char const * * argv)
    -> int
{
    auto const arguments = std::vector<std::string>{argv, argv + argc};

    auto const enableDebugOutput = isDebugOutputEnabled(arguments);

    try
    {
        auto app = Application{enableDebugOutput};

        app.run();
    }
    catch (std::exception const & error)
    {
        std::cout << error.what() << "\n";
    }
}