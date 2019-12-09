#pragma once

#include <Glow/Query/Query.hpp>

#include <chrono>
#include <optional>

namespace glow
{

class TimestampQuery : public Query
{

public:

    using Nanoseconds = std::chrono::nanoseconds;

public:

    TimestampQuery();

    auto scheduleTimestampRecording()
        -> void;

    auto waitForResultAndFetch() const
        -> Nanoseconds;

    auto fetchResultIfAvailable() const
        -> std::optional<Nanoseconds>;

};

} // namespace glow
