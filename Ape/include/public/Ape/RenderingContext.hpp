#pragma once

namespace ape
{

enum class RenderingPolicy
{

    doNotUseArrayObjects,

    useArrayObjects

};

class RenderingContext
{

public:

    RenderingContext(int const id, RenderingPolicy const policy)
        : id{id}
        , policy{policy}
    {
    }

public:

    int id;

    RenderingPolicy policy;

};

} // namespace ape
