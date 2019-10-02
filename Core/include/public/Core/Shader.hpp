#pragma once

#include <stdexcept>
#include <string>

namespace ape
{

class CouldNotCompileShader : public std::logic_error
{

public:

    explicit CouldNotCompileShader(std::string message)
        : logic_error{"Failed to compile vertex shader: " + std::move(message)}
    {
    }

};

class Shader
{

public:

    Shader(Shader const & rhs) = delete;

    Shader(Shader && rhs) noexcept;

    auto operator = (Shader const & rhs)
        -> Shader & = delete;

    auto operator = (Shader && rhs) noexcept
        -> Shader &;

    ~Shader();

    auto getId() const
        -> unsigned int;

    auto getSourceCode() const
        -> std::string;

    auto recompile(std::string newSourceCode)
        -> void;

    auto release()
        -> unsigned int;

protected:

    explicit Shader(unsigned int id, std::string sourceCode);

private:

    auto compile(std::string const & newSourceCode)
        -> void;

    auto destroy()
        -> void;

private:

    unsigned int id;

    std::string sourceCode;

};

} // namespace ape
