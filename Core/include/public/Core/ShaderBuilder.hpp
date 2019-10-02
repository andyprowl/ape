#pragma once

#include <Core/ShaderPreprocessor.hpp>
#include <Core/ShaderProgram.hpp>
#include <Core/ShaderReader.hpp>

namespace ape
{

class ShaderBuilder
{

public:

    ShaderBuilder();

    auto getReader()
        -> ShaderReader &;

    auto getReader() const
        -> ShaderReader const &;

    auto getPreprocessor() const
        -> ShaderPreprocessor const &;

    auto buildProgram(
        std::filesystem::path const & vertexShaderPath,
        std::filesystem::path const & fragmentShaderPath) const
        -> ShaderProgram;

    auto buildProgram(
        std::filesystem::path const & vertexShaderPath,
        std::filesystem::path const & geometryShaderPath,
        std::filesystem::path const & fragmentShaderPath) const
        -> ShaderProgram;

    auto buildVertexShader(std::filesystem::path const & path) const
        -> VertexShader;

    auto buildGeometryShader(std::filesystem::path const & path) const
        -> GeometryShader;

    auto buildFragmentShader(std::filesystem::path const & path) const
        -> FragmentShader;

    auto preprocessShader(std::filesystem::path const & path) const
        -> std::string;

private:

    template<typename ShaderType>
    auto buildShader(std::filesystem::path const & path) const
        -> ShaderType;

private:

    ShaderReader reader;

    ShaderPreprocessor preprocessor;

};

} // namespace ape