#pragma once

#include <Glow/Shader/ShaderPath.hpp>
#include <Glow/Shader/ShaderPreprocessor.hpp>
#include <Glow/Shader/ShaderProgram.hpp>
#include <Glow/Shader/ShaderReader.hpp>

#include <filesystem>
#include <string_view>
#include <vector>

namespace glow
{

class ShaderBuilder
{

public:

    ShaderBuilder();

    explicit ShaderBuilder(std::vector<std::filesystem::path> searchPaths);

    auto getReader()
        -> ShaderReader &;

    auto getReader() const
        -> ShaderReader const &;

    auto getPreprocessor() const
        -> ShaderPreprocessor const &;

    auto buildProgram(
        VertexShaderPath const & vertexShaderPath,
        FragmentShaderPath const & fragmentShaderPath,
        std::string_view label = "") const
        -> ShaderProgram;

    auto buildProgram(
        VertexShaderPath const & vertexShaderPath,
        GeometryShaderPath const & geometryShaderPath,
        FragmentShaderPath const & fragmentShaderPath,
        std::string_view label = "") const
        -> ShaderProgram;

    auto buildVertexShader(std::filesystem::path const & path, std::string_view label = "") const
        -> VertexShader;

    auto buildGeometryShader(std::filesystem::path const & path, std::string_view label = "") const
        -> GeometryShader;

    auto buildFragmentShader(std::filesystem::path const & path, std::string_view label = "") const
        -> FragmentShader;

    auto preprocessShader(std::filesystem::path const & path) const
        -> std::string;

private:

    template<typename ShaderType>
    auto buildShader(std::filesystem::path const & path, std::string_view label) const
        -> ShaderType;

private:

    ShaderReader reader;

    ShaderPreprocessor preprocessor;

};

} // namespace glow
