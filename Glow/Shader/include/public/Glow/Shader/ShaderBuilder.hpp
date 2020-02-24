#pragma once

#include <Glow/Shader/ShaderPath.hpp>
#include <Glow/Shader/ShaderPreprocessor.hpp>
#include <Glow/Shader/ShaderProgram.hpp>
#include <Glow/Shader/ShaderReader.hpp>

#include <filesystem>
#include <string_view>
#include <vector>

namespace glow::logging
{

class ShaderBuilderLogger;

} // namespace glow::logging

namespace glow
{

class ShaderBuilder
{

public:

    explicit ShaderBuilder(logging::ShaderBuilderLogger & logger);

    ShaderBuilder(
        std::vector<std::filesystem::path> searchPaths,
        logging::ShaderBuilderLogger & logger);

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

    auto buildProgram(
        std::vector<VertexShaderPath> const & vertexShaderPaths,
        std::vector<GeometryShaderPath> const & geometryShaderPaths,
        std::vector<FragmentShaderPath> const & fragmentShaderPaths,
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

    logging::ShaderBuilderLogger * logger;

    ShaderReader reader;

    ShaderPreprocessor preprocessor;

};

} // namespace glow
