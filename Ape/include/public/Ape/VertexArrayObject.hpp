#pragma once

#include <Ape/BufferObject.hpp>

namespace ape
{

class VertexArrayObject : public BufferObject
{

public:

    enum class ConstructionPolicy
    {

        bind,

        doNotBind

    };

public:

    VertexArrayObject();

    auto bind() const
        -> void;

    auto unbind() const
        -> void;

};

} // namespace ape
