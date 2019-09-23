#pragma once

#ifdef _WIN32

#   include <Windows.h>

#endif

#include <gl/GL.h>

namespace ape
{

class OpenGLGateway
{

public:

    virtual ~OpenGLGateway() = default;

    virtual auto glVertexAttribPointer(
        GLuint index,
        GLint size,
        GLenum type,
        GLboolean normalized,
        GLsizei stride,
        void * const pointer)
        -> void = 0;

    /*
    glEnableVertexAttribArray
    glGenBuffers
    glBindBuffer
    glBufferData
    glGenVertexArrays
    glBindVertexArray
    glDeleteVertexArrays
    glEnable
    glBindVertexArray
    glDrawElements
    glGetUniformiv
    glUniform1i
    glGetUniformfv
    glUniform1f
    glGetUniformiv
    glUniformMatrix3fv
    glUniformMatrix4fv
    glUniform2f
    glUniform3f
    glUniform4f
    glGetShaderiv
    glGetShaderInfoLog
    glGetProgramiv
    glGetProgramInfoLog
    glCreateShader
    glShaderSource
    glCompileShader
    glCreateProgram
    glAttachShader
    glLinkProgram
    glDeleteShader
    glUseProgram
    glGetUniformLocation
    glClearColor
    glClear
    glPolygonMode
    glGenTextures
    glBindTexture
    glTexParameteri
    glTexImage2D
    glGenerateMipmap
    glActiveTexture
    glBindTexture
    glViewport
    */

};

} // namespace ape
