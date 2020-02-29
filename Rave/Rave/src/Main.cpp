#include "Application.hpp"

#include <Basix/Range/Search.hpp>

#include <iostream>
#include <exception>
#include <string>
#include <vector>

auto isDebugOutputEnabled(std::vector<std::string> const & arguments)
    -> bool
{
    return basix::contains(arguments, "--enable-debug-output");
}

auto isSponzaExcluded(std::vector<std::string> const & arguments)
    -> bool
{
    return basix::contains(arguments, "--exclude-sponza");
}

auto isTextureCompressionEnabled(std::vector<std::string> const & arguments)
    -> bool
{
    return basix::contains(arguments, "--texture-compression");
}

auto main(int argc, char const * * argv)
    -> int
{
    auto const arguments = std::vector<std::string>{argv, argv + argc};

    auto const enableDebugOutput = isDebugOutputEnabled(arguments);

    auto const excludeSponza = isSponzaExcluded(arguments);

    auto const enableTextureCompression = isTextureCompressionEnabled(arguments);

    try
    {
        auto app = rave::Application{enableDebugOutput, excludeSponza, enableTextureCompression};

        app.run();
    }
    catch (std::exception const & error)
    {
        std::cout << error.what() << "\n";
    }
}
