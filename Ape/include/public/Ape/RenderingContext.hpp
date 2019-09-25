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

    explicit RenderingContext(RenderingPolicy const policy)
        : id{nextId++}
        , policy{policy}
    {
    }

public:

    auto getId() const
        -> int
    {
        return id;
    }

    auto getPolicy() const
        -> RenderingPolicy
    {
        return policy;
    }

private:

    static inline auto nextId = 0;

private:

    int id;

    RenderingPolicy policy;

};

} // namespace ape
