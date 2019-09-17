#pragma once

#include "Position.hpp"
#include "Size.hpp"

#include "GLFW.hpp"

#include <stdexcept>
#include <string>

class CouldNotCreateWindow : public std::logic_error
{

public:

    CouldNotCreateWindow()
        : logic_error{"Failed to create the OpenGL window"}
    {
    }

};

class CouldNotInitializeGLAD : public std::logic_error
{

public:

    CouldNotInitializeGLAD()
        : logic_error{"Failed to create initialize the GLAD framework"}
    {
    }

};

auto createWindow(std::string const & title, bool fullScreen)
    -> GLFWwindow &;

auto getWindowRatio(GLFWwindow & window)
    -> float;

auto getWindowCenter(GLFWwindow & window)
    -> Position;

auto getWindowSize(GLFWwindow & window)
    -> Size<int>;
