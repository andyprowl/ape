#pragma once

namespace glow
{

enum class FrameBufferAttachment
{
    depth,
    stencil,
    depthAndStencil,
    color0,
    color1,
    color2,
    color3,
    color4,
    color5,
    color6,
    color7
};

inline auto getColorAttachmentCode(int const index)
    -> FrameBufferAttachment
{
    auto const base = static_cast<int>(FrameBufferAttachment::color0);

    return static_cast<FrameBufferAttachment>(base + index);
}

} // namespace glow
