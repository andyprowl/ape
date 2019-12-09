#include <Glow/Query/Query.hpp>

#include <glad/glad.h>

#include <cassert>

namespace glow
{

namespace
{

auto createQueryResource()
    -> GpuResource
{
    auto id = GpuResource::Id{};

    glGenQueries(1, &id);

    return GpuResource{id, [] (GpuResource::Id const id) { glDeleteQueries(1, &id); }};
}

} // unnamed namespace

auto Query::getId() const
    -> GpuResource::Id
{
    return resource.get();
}

auto Query::waitForResultAndFetch() const
    -> ResultType
{
    auto const id = getId();

    auto result = GLuint64{0};

    glGetQueryObjectui64v(id, GL_QUERY_RESULT, &result);

    return result;
}

auto Query::fetchResultIfAvailable() const
    -> std::optional<ResultType>
{
    auto const id = getId();

    auto result = static_cast<GLuint64>(-1);

    glGetQueryObjectui64v(id, GL_QUERY_RESULT_NO_WAIT, &result);

    if (result == static_cast<GLuint64>(-1))
    {
        return std::nullopt;
    }
    else
    {
        return result;
    }
}

auto Query::isResultAvailable() const
    -> bool
{
    auto const id = getId();

    auto result = GLint{0};

    glGetQueryObjectiv(id, GL_QUERY_RESULT_AVAILABLE, &result);

    return (result == GL_TRUE);
}

Query::Query()
    : resource{createQueryResource()}
{
}

} // namespace glow
