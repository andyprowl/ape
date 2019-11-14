#pragma once

#include <stdexcept>

namespace ape
{

class CouldNotLoadOpenGLFunctions : public std::logic_error
{

public:

    CouldNotLoadOpenGLFunctions()
        : logic_error{"Failed to load OpenGL functions"}
    {
    }

};

class OpenGLLoader
{

public:

    OpenGLLoader();

    OpenGLLoader(bool loadImmediately, bool enableDebugOutput);

    auto loadFunctions()
        -> void;

    auto areFunctionsLoaded() const
        -> bool;

private:

    auto prepareDebugOutput()
        -> void;

private:

    bool enableDebugOutput;

    bool areOpenGLFunctionsLoaded;

};

} // namespace ape
