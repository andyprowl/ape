#pragma once

namespace ape
{

using GLenum = unsigned int;

enum class PixelType
{
    unknown,
    unsignedByte,
    floatingPoint
};

auto convertToOpenGLPixelType(PixelType type)
    -> GLenum;

auto convertFromOpenGLPixelType(GLenum type)
    -> PixelType;

} // namespace ape
