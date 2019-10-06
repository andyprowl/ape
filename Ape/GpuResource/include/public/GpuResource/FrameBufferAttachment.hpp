#pragma once

namespace ape
{

enum class FrameBufferAttachment
{
    depth,
    stencil,
    depthAndStencil,
    color0
};

auto getColorAttachmentCode(int const index)
    -> FrameBufferAttachment
{
    auto const base = static_cast<int>(FrameBufferAttachment::color0);

    return static_cast<FrameBufferAttachment>(base + index);
}

} // namespace ape
