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

BasicUniform::BasicUniform(ShaderProgram & shader, std::string_view name)
    : shader{&shader}
    , location{getUniformLocation(name)}
{
}

auto BasicUniform::getShader() const
    -> ShaderProgram &
{
    return *shader;
}

auto BasicUniform::getLocation() const
    -> int
{
    return location;
}

auto BasicUniform::getUniformLocation(std::string_view const name) const
    -> int
{
    auto const shaderId = shader->getId();

    auto const uniformLocation = glGetUniformLocation(shaderId, name.data());

    if (uniformLocation < 0)
    {
        throw UniformNotFound{std::string{name}};
    }

    return uniformLocation;
}

} // namespace glow::detail

namespace glow
{

template<>
auto Uniform<bool>::get() const
    -> bool
{
    auto const shaderId = getShader().getId();

    auto const uniformLocation = getLocation();

    auto value = int{};

    glGetUniformiv(shaderId, uniformLocation, &value);

    assert(glGetError() == GL_NO_ERROR);

    return (value != 0);
}

template<>
auto Uniform<bool>::set(bool const & value)
    -> void
{
    auto const uniformLocation = getLocation();

    glUniform1i(uniformLocation, value);

    assert(glGetError() == GL_NO_ERROR);
}

template<>
auto Uniform<float>::get() const
    -> float
{
    auto const shaderId = getShader().getId();

    auto const uniformLocation = getLocation();

    auto value = float{0.0f};

    glGetUniformfv(shaderId, uniformLocation, &value);

    assert(glGetError() == GL_NO_ERROR);

    return value;
}

template<>
auto Uniform<float>::set(float const & value)
    -> void
{
    auto const uniformLocation = getLocation();

    glUniform1f(uniformLocation, value);

    assert(glGetError() == GL_NO_ERROR);
}

template<>
auto Uniform<int>::get() const
    -> int
{
    auto const shaderId = getShader().getId();

    auto const uniformLocation = getLocation();

    auto value = int{};

    glGetUniformiv(shaderId, uniformLocation, &value);

    assert(glGetError() == GL_NO_ERROR);

    return value;
}

template<>
auto Uniform<int>::set(int const & value)
    -> void
{
    auto const uniformLocation = getLocation();

    glUniform1i(uniformLocation, value);

    assert(glGetError() == GL_NO_ERROR);
}

template<>
auto Uniform<glm::mat3>::get() const
    -> glm::mat3
{
    auto const shaderId = getShader().getId();

    auto const uniformLocation = getLocation();

    auto value = glm::mat3{0.0f};

    glGetUniformfv(shaderId, uniformLocation, glm::value_ptr(value));

    assert(glGetError() == GL_NO_ERROR);

    return value;
}

template<>
auto Uniform<glm::mat3>::set(glm::mat3 const & value)
    -> void
{
    auto const uniformLocation = getLocation();

    glUniformMatrix3fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(value));

    assert(glGetError() == GL_NO_ERROR);
}

template<>
auto Uniform<glm::mat4>::get() const
    -> glm::mat4
{
    auto const shaderId = getShader().getId();

    auto const uniformLocation = getLocation();

    auto value = glm::mat4{0.0f};

    glGetUniformfv(shaderId, uniformLocation, glm::value_ptr(value));

    assert(glGetError() == GL_NO_ERROR);

    return value;
}

template<>
auto Uniform<glm::mat4>::set(glm::mat4 const & value)
    -> void
{
    auto const uniformLocation = getLocation();

    glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(value));

    assert(glGetError() == GL_NO_ERROR);
}

template<>
auto Uniform<glm::vec2>::get() const
    -> glm::vec2
{
    auto const shaderId = getShader().getId();

    auto const uniformLocation = getLocation();

    auto value = std::array<float, 2u>{0};

    glGetUniformfv(shaderId, uniformLocation, value.data());

    assert(glGetError() == GL_NO_ERROR);

    return glm::vec2{value[0], value[1]};
}

template<>
auto Uniform<glm::vec2>::set(glm::vec2 const & value)
    -> void
{
    auto const uniformLocation = getLocation();

    glUniform2f(uniformLocation, value.x, value.y);

    assert(glGetError() == GL_NO_ERROR);
}

template<>
auto Uniform<glm::ivec2>::get() const
    -> glm::ivec2
{
    auto const shaderId = getShader().getId();

    auto const uniformLocation = getLocation();

    auto value = std::array<int, 2u>{0};

    glGetUniformiv(shaderId, uniformLocation, value.data());

    assert(glGetError() == GL_NO_ERROR);

    return glm::ivec2{value[0], value[1]};
}

template<>
auto Uniform<glm::ivec2>::set(glm::ivec2 const & value)
    -> void
{
    auto const uniformLocation = getLocation();

    glUniform2i(uniformLocation, value.x, value.y);

    assert(glGetError() == GL_NO_ERROR);
}

template<>
auto Uniform<glm::vec3>::get() const
    -> glm::vec3
{
    auto const shaderId = getShader().getId();

    auto const uniformLocation = getLocation();

    auto value = std::array<float, 3u>{0};

    glGetUniformfv(shaderId, uniformLocation, value.data());

    assert(glGetError() == GL_NO_ERROR);

    return glm::vec3{value[0], value[1], value[2]};
}

template<>
auto Uniform<glm::vec3>::set(glm::vec3 const & value)
    -> void
{
    auto const uniformLocation = getLocation();

    glUniform3f(uniformLocation, value.x, value.y, value.z);

    assert(glGetError() == GL_NO_ERROR);
}

template<>
auto Uniform<glm::ivec3>::get() const
    -> glm::ivec3
{
    auto const shaderId = getShader().getId();

    auto const uniformLocation = getLocation();

    auto value = std::array<int, 3u>{0};

    glGetUniformiv(shaderId, uniformLocation, value.data());

    assert(glGetError() == GL_NO_ERROR);

    return glm::ivec3{value[0], value[1], value[2]};
}

template<>
auto Uniform<glm::ivec3>::set(glm::ivec3 const & value)
    -> void
{
    auto const uniformLocation = getLocation();

    glUniform3i(uniformLocation, value.x, value.y, value.z);

    assert(glGetError() == GL_NO_ERROR);
}

template<>
auto Uniform<glm::vec4>::get() const
    -> glm::vec4
{
    auto const shaderId = getShader().getId();

    auto const uniformLocation = getLocation();

    auto value = std::array<float, 4u>{0};

    glGetUniformfv(shaderId, uniformLocation, value.data());

    assert(glGetError() == GL_NO_ERROR);

    return glm::vec4{value[0], value[1], value[2], value[3]};
}

template<>
auto Uniform<glm::vec4>::set(glm::vec4 const & value)
    -> void
{
    auto const uniformLocation = getLocation();

    glUniform4f(uniformLocation, value.x, value.y, value.z, value.w);

    assert(glGetError() == GL_NO_ERROR);
}

template<>
auto Uniform<glm::ivec4>::get() const
    -> glm::ivec4
{
    auto const shaderId = getShader().getId();

    auto const uniformLocation = getLocation();

    auto value = std::array<int, 4u>{0};

    glGetUniformiv(shaderId, uniformLocation, value.data());

    assert(glGetError() == GL_NO_ERROR);

    return glm::ivec4{value[0], value[1], value[2], value[3]};
}

template<>
auto Uniform<glm::ivec4>::set(glm::ivec4 const & value)
    -> void
{
    auto const uniformLocation = getLocation();

    glUniform4i(uniformLocation, value.x, value.y, value.z, value.w);

    assert(glGetError() == GL_NO_ERROR);
}

template class Uniform<bool>;

template class Uniform<float>;

template class Uniform<int>;

template class Uniform<glm::mat3>;

template class Uniform<glm::mat4>;

template class Uniform<glm::vec2>;

template class Uniform<glm::ivec2>;

template class Uniform<glm::vec3>;

template class Uniform<glm::ivec3>;

template class Uniform<glm::vec4>;

template class Uniform<glm::ivec4>;

} // namespace glow::detail
