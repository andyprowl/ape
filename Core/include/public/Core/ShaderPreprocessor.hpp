#pragma once

#include <filesystem>
#include <string>

namespace ape
{

class ShaderReader;

class ShaderPreprocessor
{

public:

    explicit ShaderPreprocessor(ShaderReader & reader);

    auto preprocess(std::string source) const
        -> std::string;

    auto getReader() const
        -> ShaderReader &;

private:

    ShaderReader * reader;

};

auto preprocessShader(ShaderPreprocessor const & preprocessor, std::filesystem::path const & path)
    -> std::string;

} // namespace ape
