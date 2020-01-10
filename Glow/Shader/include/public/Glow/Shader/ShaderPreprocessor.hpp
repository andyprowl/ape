#pragma once

#include <filesystem>
#include <string>
#include <string_view>

namespace glow
{

class ShaderReader;

class IncludeDirectiveNotAtLineStart : public std::exception
{

public:

    // virtual (from std::exception)
    auto what() const noexcept
        -> const char * override
    {
        return "An #include directive can only appear at the beginning of a shader line";
    }

};

class ShaderPreprocessor
{

public:

    explicit ShaderPreprocessor(ShaderReader & reader);

    auto preprocess(std::string source) const
        -> std::string;

    auto getReader() const
        -> ShaderReader &;

private:

    auto findIncludeDirectiveIndex(std::string_view source, std::size_t startIndex) const
        -> std::size_t;

private:

    ShaderReader * reader;

};

auto preprocessShader(ShaderPreprocessor const & preprocessor, std::filesystem::path const & path)
    -> std::string;

} // namespace glow
