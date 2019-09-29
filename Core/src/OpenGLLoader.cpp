#include <Core/OpenGLLoader.hpp>

#include <glad/glad.h>

#include <cassert>
#include <iostream>

namespace ape
{

namespace
{

auto GLAPIENTRY onOpenGLError(
    GLenum const /*source*/,
    GLenum const type,
    GLuint const /*id*/,
    GLenum const severity,
    GLsizei const /*length*/,
    GLchar const * const message,
    void const * const /*userParam*/)
    -> void
{
    std::cout 
        << "OpenGL callback invoked! "
        << (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "")
        << " type = 0x" << std::hex << type 
        << ", severity = 0x" << std::hex << severity
        << ", message = " << message << "\n";
}

} // unnamed namespace

OpenGLLoader::OpenGLLoader()
    : OpenGLLoader{false, false}
{
}

OpenGLLoader::OpenGLLoader(bool const loadImmediately, bool const enableDebugOutput)
    : enableDebugOutput{enableDebugOutput}
    , areFunctionsLoaded{false}
{
    if (loadImmediately)
    {
        loadFunctions();
    }
}

auto OpenGLLoader::loadFunctions()
    -> void
{
    if (areFunctionsLoaded)
    {
        return;
    }

    if (!gladLoadGL())
    {
        throw CouldNotLoadOpenGLFunctions{};
    }

    areFunctionsLoaded = true;

    if (enableDebugOutput)
    {
        prepareDebugOutput();
    }
}

auto OpenGLLoader::prepareDebugOutput()
    -> void
{
    glEnable(GL_DEBUG_OUTPUT);
    
    glDebugMessageCallback(onOpenGLError, 0);

    assert(glGetError() == GL_NO_ERROR);
}

} // namespace ape
