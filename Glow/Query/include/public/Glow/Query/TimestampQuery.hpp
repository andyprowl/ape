#pragma once

#include <Glow/Query/Query.hpp>

namespace glow
{

class TimestampQuery : public Query
{

public:

    auto setCurrentTimeIntoResult()
        -> void;

};

} // namespace glow
