#pragma once

#include <array>

namespace glow
{

using GLenum = unsigned int;

enum class CubeTextureFace
{
    right,
    left,
    top,
    bottom,
    front,
    back
};

constexpr auto getCubeTextureFaces()
    -> std::array<CubeTextureFace, 6u>
{
    return {{
        CubeTextureFace::right,
        CubeTextureFace::left,
        CubeTextureFace::top,
        CubeTextureFace::bottom,
        CubeTextureFace::front,
        CubeTextureFace::back}};
}

auto convertToOpenGLFace(CubeTextureFace face)
    -> GLenum;

} // namespace glow
