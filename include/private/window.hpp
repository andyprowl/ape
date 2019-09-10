#pragma once

#include "glad/glad.h"

#include <GLFW/glfw3.h>

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

auto createWindow()
    -> GLFWwindow &;

auto getWindowRatio(GLFWwindow & window)
    -> float;
