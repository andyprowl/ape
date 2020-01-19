#pragma once

#include <Glow/BufferObject/DataType.hpp>

#include <cstddef>

namespace glow
{

class VertexAttribute
{

public:

    VertexAttribute(
        DataType const type,
        int const numOfComponents,
        std::size_t const relativeOffset,
        int const stride)
        : type{type}
        , numOfComponents{numOfComponents}
        , relativeOffset{relativeOffset}
        , stride{stride}
    {
    }

public:

    DataType type;

    int numOfComponents;

    std::size_t relativeOffset;

    int stride;

};

} // namespace glow
