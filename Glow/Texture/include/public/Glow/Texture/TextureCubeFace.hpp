#pragma once

#include <array>

namespace glow
{

using GLenum = unsigned int;

enum class TextureCubeFace
{
    right,
    left,
    top,
    bottom,
    front,
    back
};

constexpr auto getCubeTextureFaces()
    -> std::array<TextureCubeFace, 6u>
{
    return {{
        TextureCubeFace::right,
        TextureCubeFace::left,
        TextureCubeFace::top,
        TextureCubeFace::bottom,
        TextureCubeFace::front,
        TextureCubeFace::back}};
}

auto convertToOpenGLFace(TextureCubeFace face)
    -> GLenum;

} // namespace glow
