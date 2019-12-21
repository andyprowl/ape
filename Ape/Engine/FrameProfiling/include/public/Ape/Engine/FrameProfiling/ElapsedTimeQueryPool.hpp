#pragma once

#include <Ape/Engine/FrameProfiling/QueryPool.hpp>

#include <Glow/Query/ElapsedTimeQuery.hpp>

namespace ape
{

class ElapsedTimeQueryPool : public QueryPool<glow::ElapsedTimeQuery>
{

public:

    using QueryRegistration = int;

public:

    using QueryPool::QueryPool;

    auto beginQuery(ResultReceiver resultReceiver)
        -> QueryRegistration;

    auto endQuery(QueryRegistration registration)
        -> void;

};

} // namespace ape
