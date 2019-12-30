#include <Glow/Shader/Uniform.hpp>

#include <Glow/Shader/ShaderProgram.hpp>

#include <Glow/GpuResource/ScopedBinder.hpp>

#include <Basix/CompilerSupport/CompilerWarnings.hpp>

#include <glad/glad.h>

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

// Non-standard extensions used in GLM
disableCompilerWarnings()
#include <glm/gtc/type_ptr.hpp>
restoreCompilerWarnings()

#include <array>
#include <cassert>

namespace glow::detail
{

template<typename T>
BasicUniform<T>::BasicUniform(ShaderProgram & program, std::string_view name)
    : BasicUniform{program.getUniform<T>(name)}
{
}

template<typename T>
BasicUniform<T>::BasicUniform(
    ShaderProgram & program,
    std::string_view name,
    T const & value)
    : BasicUniform<T>{program, name}
{
    auto const shaderBinder = bind(program);

    set(value);
}

template<>
auto BasicUniform<bool>::get() const
    -> bool
{
    auto value = int{};

    glGetUniformiv(programId, location, &value);

    assert(glGetError() == GL_NO_ERROR);

    return (value != 0);
}

template<>
auto BasicUniform<bool>::set(bool const & value)
    -> void
{
    glUniform1i(location, value);

    assert(glGetError() == GL_NO_ERROR);
}

template<>
auto BasicUniform<float>::get() const
    -> float
{
    auto value = float{0.0f};

    glGetUniformfv(programId, location, &value);

    assert(glGetError() == GL_NO_ERROR);

    return value;
}

template<>
auto BasicUniform<float>::set(float const & value)
    -> void
{
    glUniform1f(location, value);

    assert(glGetError() == GL_NO_ERROR);
}

template<>
auto BasicUniform<int>::get() const
    -> int
{
    auto value = int{};

    glGetUniformiv(programId, location, &value);

    assert(glGetError() == GL_NO_ERROR);

    return value;
}

template<>
auto BasicUniform<int>::set(int const & value)
    -> void
{
    glUniform1i(location, value);

    assert(glGetError() == GL_NO_ERROR);
}

template<>
auto BasicUniform<glm::mat3>::get() const
    -> glm::mat3
{
    auto value = glm::mat3{0.0f};

    glGetUniformfv(programId, location, glm::value_ptr(value));

    assert(glGetError() == GL_NO_ERROR);

    return value;
}

template<>
auto BasicUniform<glm::mat3>::set(glm::mat3 const & value)
    -> void
{
    glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));

    assert(glGetError() == GL_NO_ERROR);
}

template<>
auto BasicUniform<glm::mat4>::get() const
    -> glm::mat4
{
    auto value = glm::mat4{0.0f};

    glGetUniformfv(programId, location, glm::value_ptr(value));

    assert(glGetError() == GL_NO_ERROR);

    return value;
}

template<>
auto BasicUniform<glm::mat4>::set(glm::mat4 const & value)
    -> void
{
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));

    assert(glGetError() == GL_NO_ERROR);
}

template<>
auto BasicUniform<glm::vec2>::get() const
    -> glm::vec2
{
    auto value = std::array<float, 2u>{0};

    glGetUniformfv(programId, location, value.data());

    assert(glGetError() == GL_NO_ERROR);

    return glm::vec2{value[0], value[1]};
}

template<>
auto BasicUniform<glm::vec2>::set(glm::vec2 const & value)
    -> void
{
    glUniform2f(location, value.x, value.y);

    assert(glGetError() == GL_NO_ERROR);
}

template<>
auto BasicUniform<glm::ivec2>::get() const
    -> glm::ivec2
{
    auto value = std::array<int, 2u>{0};

    glGetUniformiv(programId, location, value.data());

    assert(glGetError() == GL_NO_ERROR);

    return glm::ivec2{value[0], value[1]};
}

template<>
auto BasicUniform<glm::ivec2>::set(glm::ivec2 const & value)
    -> void
{
    glUniform2i(location, value.x, value.y);

    assert(glGetError() == GL_NO_ERROR);
}

template<>
auto BasicUniform<glm::vec3>::get() const
    -> glm::vec3
{
    auto value = std::array<float, 3u>{0};

    glGetUniformfv(programId, location, value.data());

    assert(glGetError() == GL_NO_ERROR);

    return glm::vec3{value[0], value[1], value[2]};
}

template<>
auto BasicUniform<glm::vec3>::set(glm::vec3 const & value)
    -> void
{
    glUniform3f(location, value.x, value.y, value.z);

    assert(glGetError() == GL_NO_ERROR);
}

template<>
auto BasicUniform<glm::ivec3>::get() const
    -> glm::ivec3
{
    auto value = std::array<int, 3u>{0};

    glGetUniformiv(programId, location, value.data());

    assert(glGetError() == GL_NO_ERROR);

    return glm::ivec3{value[0], value[1], value[2]};
}

template<>
auto BasicUniform<glm::ivec3>::set(glm::ivec3 const & value)
    -> void
{
    glUniform3i(location, value.x, value.y, value.z);

    assert(glGetError() == GL_NO_ERROR);
}

template<>
auto BasicUniform<glm::vec4>::get() const
    -> glm::vec4
{
    auto value = std::array<float, 4u>{0};

    glGetUniformfv(programId, location, value.data());

    assert(glGetError() == GL_NO_ERROR);

    return glm::vec4{value[0], value[1], value[2], value[3]};
}

template<>
auto BasicUniform<glm::vec4>::set(glm::vec4 const & value)
    -> void
{
    glUniform4f(location, value.x, value.y, value.z, value.w);

    assert(glGetError() == GL_NO_ERROR);
}

template<>
auto BasicUniform<glm::ivec4>::get() const
    -> glm::ivec4
{
    auto value = std::array<int, 4u>{0};

    glGetUniformiv(programId, location, value.data());

    assert(glGetError() == GL_NO_ERROR);

    return glm::ivec4{value[0], value[1], value[2], value[3]};
}

template<>
auto BasicUniform<glm::ivec4>::set(glm::ivec4 const & value)
    -> void
{
    glUniform4i(location, value.x, value.y, value.z, value.w);

    assert(glGetError() == GL_NO_ERROR);
}


template class BasicUniform<bool>;

template class BasicUniform<float>;

template class BasicUniform<int>;

template class BasicUniform<glm::mat3>;

template class BasicUniform<glm::mat4>;

template class BasicUniform<glm::vec2>;

template class BasicUniform<glm::ivec2>;

template class BasicUniform<glm::vec3>;

template class BasicUniform<glm::ivec3>;

template class BasicUniform<glm::vec4>;

template class BasicUniform<glm::ivec4>;

} // namespace glow::detail
