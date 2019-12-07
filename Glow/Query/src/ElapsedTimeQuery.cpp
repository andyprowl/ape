#include <Glow/Query/ElapsedTimeQuery.hpp>

namespace glow
{

ElapsedTimeQuery::ElapsedTimeQuery()
    : ScopedQuery{ScopedQueryType::timeElapsed}
{
}

auto ElapsedTimeQuery::waitForResultAndFetch() const
    -> Nanoseconds
{
    auto const result = ScopedQuery::waitForResultAndFetch();

    return Nanoseconds{result};
}

auto ElapsedTimeQuery::fetchResultIfAvailable() const
    -> std::optional<Nanoseconds>
{
    auto const maybeResult = ScopedQuery::fetchResultIfAvailable();

    return std::optional<Nanoseconds>{maybeResult};
}

} // namespace glow
