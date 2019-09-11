#include "application.hpp"

#include <iostream>
#include <exception>

auto main()
    -> int
{
    try
    {
        auto && app = Application{};

        app.run();
    }
    catch (std::exception const & error)
    {
        std::cout << error.what() << "\n";
    }
}