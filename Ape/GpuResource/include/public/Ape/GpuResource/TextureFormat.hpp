#pragma once

namespace ape
{

using GLenum = unsigned int;

enum class TextureFormat
{
    unknown,
    redOnly,
    redGreenOnly,
    redGreenBlue,
    redGreenBlueAlpha,
    depthOnly,
    depthAndStencil
};

auto convertToOpenGLFormat(TextureFormat format)
    -> GLenum;

auto convertFromOpenGLFormat(GLenum format)
    -> TextureFormat;

} // namespace ape
