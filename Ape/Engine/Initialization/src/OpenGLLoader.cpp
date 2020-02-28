#include <Ape/Engine/Initialization/OpenGLLoader.hpp>

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
    if (severity == GL_DEBUG_SEVERITY_NOTIFICATION)
    {
        return;
    }

    // Hack: ignore warning about undefined behavior of unused samplers
    if (type == 0x824e)
    {
        return;
    }

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
    , areOpenGLFunctionsLoaded{false}
{
    if (loadImmediately)
    {
        loadFunctions();
    }
}

auto OpenGLLoader::loadFunctions()
    -> void
{
    if (areOpenGLFunctionsLoaded)
    {
        return;
    }

    if (not gladLoadGL())
    {
        throw CouldNotLoadOpenGLFunctions{};
    }

    areOpenGLFunctionsLoaded = true;

    if (enableDebugOutput)
    {
        prepareDebugOutput();
    }
}

auto OpenGLLoader::areFunctionsLoaded() const
    -> bool
{
    return areOpenGLFunctionsLoaded;
}

auto OpenGLLoader::prepareDebugOutput()
    -> void
{
    glEnable(GL_DEBUG_OUTPUT);

    // This will make sure the error handler is invoked in the same thread where the error occurs,
    // so that the call stack is not lost and debugging is simpler.
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    
    glDebugMessageCallback(onOpenGLError, 0);

    assert(glGetError() == GL_NO_ERROR);
}

} // namespace ape
