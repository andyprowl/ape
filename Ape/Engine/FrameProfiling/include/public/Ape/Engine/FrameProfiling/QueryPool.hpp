#pragma once

#include <Glow/Query/Query.hpp>

#include <chrono>
#include <functional>
#include <type_traits>
#include <vector>

namespace ape
{

template<typename QueryType>
class QueryPool
{

public:

    static_assert(
        std::is_base_of_v<glow::Query, QueryType>,
        "QueryType must derive from glow::Query!");

    using QueryResultType = typename QueryType::ResultType;

    using ResultReceiver = std::function<auto (QueryResultType) -> void>;

public:

    QueryPool() = default;

    explicit QueryPool(int const numOfPreCreatedQueries)
        : queries{numOfPreCreatedQueries}
    {
    }

    auto fetchAndStoreAllResults()
        -> void
    {
        for (auto & recording : recordings)
        {
            auto const result = recording.query.waitForResultAndFetch();

            recording.resultReceiver(result);

            queries.push_back(std::move(recording.query));
        }
    
        recordings.clear();
    }

protected:

    class TimestampRecording
    {

    public:

        TimestampRecording(QueryType query, ResultReceiver resultReceiver)
            : query{std::move(query)}
            , resultReceiver{std::move(resultReceiver)}
        {
        }

    public:

        QueryType query;

        ResultReceiver resultReceiver;

    };
    
protected:

    auto fetchAvailableQuery()
        -> QueryType
    {
        if (queries.empty())
        {
            return QueryType{};
        }

        auto query = std::move(queries.back());

        queries.pop_back();

        return query;
    }

protected:

    std::vector<QueryType> queries;

    std::vector<TimestampRecording> recordings;

};


} // namespace ape
