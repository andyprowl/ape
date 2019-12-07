#include <Glow/Query/TimestampQuery.hpp>

#include <glad/glad.h>

namespace glow
{

auto TimestampQuery::setCurrentTimeIntoResult()
    -> void
{
    auto const id = getId();

    glQueryCounter(id, GL_TIMESTAMP);
}

} // namespace glow
