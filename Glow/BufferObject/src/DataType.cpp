#include <Glow/BufferObject/DataType.hpp>

#include <glad/glad.h>

#include <array>

namespace glow
{

namespace
{

auto const dataTypeMap = std::array<GLenum, 8u>{{
    GL_BYTE,
    GL_UNSIGNED_BYTE,
    GL_SHORT,
    GL_UNSIGNED_SHORT,
    GL_INT,
    GL_UNSIGNED_INT,
    GL_FLOAT,
    GL_DOUBLE}};

auto const dataTypeSizeMap = std::array<int, 8u>{{
    static_cast<int>(sizeof(GLbyte)),
    static_cast<int>(sizeof(GLubyte)),
    static_cast<int>(sizeof(GLshort)),
    static_cast<int>(sizeof(GLushort)),
    static_cast<int>(sizeof(GLint)),
    static_cast<int>(sizeof(GLuint)),
    static_cast<int>(sizeof(GLfloat)),
    static_cast<int>(sizeof(GLdouble))}};

} // unnamed namespace

auto convertToOpenGLDataType(DataType const type)
    -> GLenum
{
    return dataTypeMap[static_cast<std::size_t>(type)];
}

auto getDataTypeSize(DataType const type)
    -> int
{
    return dataTypeSizeMap[static_cast<std::size_t>(type)];
}

} // namespace glow
