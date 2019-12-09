#include <Glow/Query/TimestampQuery.hpp>

#include <glad/glad.h>

namespace glow
{

TimestampQuery::TimestampQuery() = default;

auto TimestampQuery::scheduleTimestampRecording()
    -> void
{
    auto const id = getId();

    glQueryCounter(id, GL_TIMESTAMP);
}

auto TimestampQuery::waitForResultAndFetch() const
    -> Nanoseconds
{
    auto const result = Query::waitForResultAndFetch();

    return Nanoseconds{result};
}

auto TimestampQuery::fetchResultIfAvailable() const
    -> std::optional<Nanoseconds>
{
    auto const maybeResult = Query::fetchResultIfAvailable();

    return std::optional<Nanoseconds>{maybeResult};
}

} // namespace glow
