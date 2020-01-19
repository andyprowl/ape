#pragma once

#include <Glow/BufferObject/DataType.hpp>

namespace glow
{

class VertexAttribute
{

public:

    VertexAttribute(
        DataType const componentType,
        int const numOfComponents,
        int const relativeOffset,
        int const stride)
        : componentType{componentType}
        , numOfComponents{numOfComponents}
        , relativeOffset{relativeOffset}
        , stride{stride}
    {
    }

public:

    DataType componentType;

    int numOfComponents;

    int relativeOffset;

    int stride;

};

} // namespace glow
