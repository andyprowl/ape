#pragma once

namespace glow
{

using GLenum = unsigned int;

enum class ScopedQueryType
{
    samplesPassed,
    anySamplesPassed,
    anySamplesPassedConservative,
    primitivesGenerated,
    transformFeedbackPrimitivesWritten,
    timeElapsed
};

auto convertToOpenGLQueryType(ScopedQueryType type)
    -> GLenum;

} // namespace glow
