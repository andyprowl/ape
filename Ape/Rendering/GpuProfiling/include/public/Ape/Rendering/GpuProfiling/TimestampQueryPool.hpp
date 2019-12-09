#pragma once

#include <Glow/Query/TimestampQuery.hpp>

#include <functional>
#include <vector>

namespace ape
{

class TimestampQueryPool
{

public:

    using Nanoseconds = std::chrono::nanoseconds;

    using ResultReceiver = std::function<auto (Nanoseconds) -> void>;

public:

    TimestampQueryPool() = default;

    explicit TimestampQueryPool(int numOfPreCreatedQueries);

    auto placeTimestampRecordingRequest(ResultReceiver resultReceiver)
        -> void;

    auto fetchAndStoreAllResults()
        -> void;

public:

    class TimestampRecording
    {

    public:

        TimestampRecording(glow::TimestampQuery query, ResultReceiver resultReceiver)
            : query{std::move(query)}
            , resultReceiver{std::move(resultReceiver)}
        {
        }

    public:

        glow::TimestampQuery query;

        ResultReceiver resultReceiver;

    };

private:

    auto fetchAvailableQuery()
        -> glow::TimestampQuery;

private:

    std::vector<glow::TimestampQuery> queries;

    std::vector<TimestampRecording> recordings;

};

} // namespace ape
