#pragma once

#include <cstddef>

namespace glow
{

class VertexComponent
{

public:

    VertexComponent(int const sizeInFloats, std::size_t const offsetInType, int const stride)
        : sizeInFloats{sizeInFloats}
        , encodedOffsetInType{reinterpret_cast<void *>(offsetInType)}
        , stride{stride}
    {
    }

public:

    int sizeInFloats;

    void * const encodedOffsetInType;

    int stride;

};

} // namespace glow
