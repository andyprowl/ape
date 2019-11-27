#pragma once

namespace glow
{

using GLenum = unsigned int;

enum class CubeTextureFace
{
    right,
    left,
    top,
    bottom,
    back,
    front
};

auto convertToOpenGLFace(CubeTextureFace face)
    -> GLenum;

} // namespace glow
