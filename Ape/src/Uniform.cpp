#include <Ape/Uniform.hpp>

#include <Ape/CompilerWarnings.hpp>
#include <Ape/ShaderProgram.hpp>

#include <glad/glad.h>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

// Non-standard extensions used in GLM
disableCompilerWarnings()
#include <glm/gtc/type_ptr.hpp>
restoreCompilerWarnings()

#include <array>

namespace detail
{

template<typename T>
BasicUniform<T>::BasicUniform(ShaderProgram & program, std::string const & name)
    : BasicUniform{program.getUniform<T>(name)}
{
}

template<typename T>
BasicUniform<T>::BasicUniform(
    ShaderProgram & program,
    std::string const & name,
    T const & value)
    : BasicUniform<T>{program, name}
{

    program.use();

    set(value);
}

template<>
auto BasicUniform<bool>::get() const
    -> bool
{
    auto value = int{};

    glGetUniformiv(programId, location, &value);

    return (value != 0);
}

template<>
auto BasicUniform<bool>::set(bool const & value)
    -> void
{
    glUniform1i(location, value);
}

template<>
auto BasicUniform<float>::get() const
    -> float
{
    auto value = float{0.0f};

    glGetUniformfv(programId, location, &value);

    return value;
}

template<>
auto BasicUniform<float>::set(float const & value)
    -> void
{
    glUniform1f(location, value);
}

template<>
auto BasicUniform<int>::get() const
    -> int
{
    auto value = int{};

    glGetUniformiv(programId, location, &value);

    return value;
}

template<>
auto BasicUniform<int>::set(int const & value)
    -> void
{
    glUniform1i(location, value);
}

template<>
auto BasicUniform<glm::mat3>::get() const
    -> glm::mat3
{
    auto value = glm::mat3{0.0f};

    glGetUniformfv(programId, location, glm::value_ptr(value));

    return value;
}

template<>
auto BasicUniform<glm::mat3>::set(glm::mat3 const & value)
    -> void
{
    glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

template<>
auto BasicUniform<glm::mat4>::get() const
    -> glm::mat4
{
    auto value = glm::mat4{0.0f};

    glGetUniformfv(programId, location, glm::value_ptr(value));

    return value;
}

template<>
auto BasicUniform<glm::mat4>::set(glm::mat4 const & value)
    -> void
{
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

template<>
auto BasicUniform<glm::vec2>::get() const
    -> glm::vec2
{
    auto value = std::array<float, 2u>{0};

    glGetUniformfv(programId, location, value.data());

    return glm::vec2{value[0], value[1]};
}

template<>
auto BasicUniform<glm::vec2>::set(glm::vec2 const & value)
    -> void
{
    glUniform2f(location, value.x, value.y);
}

template<>
auto BasicUniform<glm::vec3>::get() const
    -> glm::vec3
{
    auto value = std::array<float, 3u>{0};

    glGetUniformfv(programId, location, value.data());

    return glm::vec3{value[0], value[1], value[2]};
}

template<>
auto BasicUniform<glm::vec3>::set(glm::vec3 const & value)
    -> void
{
    glUniform3f(location, value.x, value.y, value.z);
}

template<>
auto BasicUniform<glm::vec4>::get() const
    -> glm::vec4
{
    auto value = std::array<float, 4u>{0};

    glGetUniformfv(programId, location, value.data());

    return glm::vec4{value[0], value[1], value[2], value[3]};
}

template<>
auto BasicUniform<glm::vec4>::set(glm::vec4 const & value)
    -> void
{
    glUniform4f(location, value.x, value.y, value.z, value.w);
}

template class BasicUniform<bool>;

template class BasicUniform<float>;

template class BasicUniform<int>;

template class BasicUniform<glm::mat3>;

template class BasicUniform<glm::mat4>;

template class BasicUniform<glm::vec2>;

template class BasicUniform<glm::vec3>;

template class BasicUniform<glm::vec4>;

} // namespace detail
