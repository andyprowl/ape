#include "Uniform.hpp"

#include "CompilerWarnings.hpp"
#include "ShaderProgram.hpp"

#include "GLFW.hpp"

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

// Non-standard extensions used in GLM
disableCompilerWarnings()
#include <glm/gtc/type_ptr.hpp>
restoreCompilerWarnings()

#include <array>

template<typename T>
Uniform<T>::Uniform(ShaderProgram const & program, std::string const & name)
    : Uniform{program.getUniform<T>(name)}
{
}

auto Uniform<bool>::get() const
    -> bool
{
    auto value = int{};

    glGetUniformiv(programId, location, &value);

    return (value != 0);
}

auto Uniform<bool>::set(bool const & value)
    -> void
{
    glUniform1i(location, value);
}

auto Uniform<float>::get() const
    -> float
{
    auto value = float{0.0f};

    glGetUniformfv(programId, location, &value);

    return value;
}

auto Uniform<float>::set(float const & value)
    -> void
{
    glUniform1f(location, value);
}

auto Uniform<int>::get() const
    -> int
{
    auto value = int{};

    glGetUniformiv(programId, location, &value);

    return value;
}

auto Uniform<int>::set(int const & value)
    -> void
{
    glUniform1i(location, value);
}

auto Uniform<glm::mat3>::get() const
    -> glm::mat3
{
    auto value = glm::mat3{0.0f};

    glGetUniformfv(programId, location, glm::value_ptr(value));

    return value;
}

auto Uniform<glm::mat3>::set(glm::mat3 const & value)
    -> void
{
    glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

auto Uniform<glm::mat4>::get() const
    -> glm::mat4
{
    auto value = glm::mat4{0.0f};

    glGetUniformfv(programId, location, glm::value_ptr(value));

    return value;
}

auto Uniform<glm::mat4>::set(glm::mat4 const & value)
    -> void
{
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

auto Uniform<glm::vec2>::get() const
    -> glm::vec2
{
    auto value = std::array<float, 2u>{0};

    glGetUniformfv(programId, location, value.data());

    return glm::vec2{value[0], value[1]};
}

auto Uniform<glm::vec2>::set(glm::vec2 const & value)
    -> void
{
    glUniform2f(location, value.x, value.y);
}

auto Uniform<glm::vec3>::get() const
    -> glm::vec3
{
    auto value = std::array<float, 3u>{0};

    glGetUniformfv(programId, location, value.data());

    return glm::vec3{value[0], value[1], value[2]};
}

auto Uniform<glm::vec3>::set(glm::vec3 const & value)
    -> void
{
    glUniform3f(location, value.x, value.y, value.z);
}

auto Uniform<glm::vec4>::get() const
    -> glm::vec4
{
    auto value = std::array<float, 4u>{0};

    glGetUniformfv(programId, location, value.data());

    return glm::vec4{value[0], value[1], value[2], value[3]};
}

auto Uniform<glm::vec4>::set(glm::vec4 const & value)
    -> void
{
    glUniform4f(location, value.x, value.y, value.z, value.w);
}

template class Uniform<bool>;

template class Uniform<float>;

template class Uniform<int>;

template class Uniform<glm::mat3>;

template class Uniform<glm::mat4>;

template class Uniform<glm::vec2>;

template class Uniform<glm::vec3>;

template class Uniform<glm::vec4>;

