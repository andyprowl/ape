#include "ShaderProgram.hpp"

#include "CompilerWarnings.hpp"
#include "GLFW.hpp"

// Non-standard extensions used in GLM
disableCompilerWarnings()
#include <glm/gtc/type_ptr.hpp>
restoreCompilerWarnings()

#include <fstream>
#include <iostream>
#include <array>

namespace
{

auto const includeDirective = std::string{"#include"};

auto extractIncludedFileName(std::string const & content, std::size_t const directiveStartIndex)
    -> std::string
{
    auto const includeKeywordEnd = directiveStartIndex + includeDirective.length();

    auto const fileNameStart = content.find('\"', includeKeywordEnd) + 1;

    auto const fileNameEnd = content.find('\"', fileNameStart + 1);

    return content.substr(fileNameStart, fileNameEnd - fileNameStart);
}

auto checkShaderCompilationOutcome(int const shaderId, std::string const & filename)
    -> void
{
    auto success = int{};

    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);

    if (success)
    {
        std::cout << "Shader << '" << filename << "' successfully compiled\n";

        return;
    }
    
    auto infoLog = std::array<char, 512>{'\0'};

    glGetShaderInfoLog(shaderId, sizeof(infoLog), nullptr, infoLog.data());

    throw CouldNotCompileShader{filename, infoLog.data()};
}

auto checkShaderLinkingOutcome(int const shaderId)
    -> void
{
    auto success = int{};

    glGetProgramiv(shaderId, GL_LINK_STATUS, &success);

    if (success)
    {
        std::cout << "Shader program successfully linked\n";

        return;
    }

    auto infoLog = std::array<char, 512>{'\0'};

    glGetProgramInfoLog(shaderId, sizeof(infoLog), nullptr, infoLog.data());

    throw CouldNotLinkShaderProgram{infoLog.data()};
}

} // unnamed namespace

auto extractBasePath(std::string const & filename)
    -> std::string
{
    auto const lastSlash = filename.find_last_of("\\/");

    if (lastSlash == std::string::npos)
    {
        return filename;
    }

    return filename.substr(0, lastSlash);
}

auto readShader(std::string const & filename)
    -> std::string
{
    auto shaderFile = std::ifstream{
        std::string{shaderFolder} + "\\" + filename,
        std::ifstream::ate | std::ifstream::binary};

    if (!shaderFile)
    {
        throw CouldNotOpenShaderFile{filename};
    }

    auto const fileSize = static_cast<std::size_t>(shaderFile.tellg());

    shaderFile.seekg(0, std::ifstream::beg);

    auto data = std::string(fileSize, '\0');

    shaderFile.read(&data[0], fileSize);

    return data;
}

auto preprocessShader(std::string content)
    -> std::string
{
    auto includeStart = content.find('\n' + includeDirective);

    while (includeStart != std::string::npos)
    {
        auto fileName = extractIncludedFileName(content, includeStart);

        auto const includedContent = readShader(fileName);
            
        auto preprocessedContent = preprocessShader(includedContent);

        auto const includeEnd = content.find('\n', includeStart + fileName.length() + 3);

        auto prologue = content.substr(0, includeStart);

        auto epilogue = content.substr(includeEnd + 1);

        content = prologue + std::move(preprocessedContent) + epilogue;

        includeStart = content.find(includeDirective, includeStart + content.length());
    }

    return content;
}

auto compileShader(std::string const & filename, int const shaderType)
    -> int
{
    auto const originalCode = readShader(filename);

    auto const preprocessedCode = preprocessShader(originalCode);

    auto const shaderId = glCreateShader(shaderType);
    
    auto const shaderCode = preprocessedCode.data();

    glShaderSource(shaderId, 1, &shaderCode, nullptr);

    glCompileShader(shaderId);

    checkShaderCompilationOutcome(shaderId, filename);

    return shaderId;
}

auto compileVertexShader(std::string const & filename)
    -> int
{
    return compileShader(filename, GL_VERTEX_SHADER);
}

auto compileFragmentShader(std::string const & filename)
    -> int
{
    return compileShader(filename, GL_FRAGMENT_SHADER);
}

auto linkShaderProgram(int const vertexShaderId, int const fragmentShaderId)
    -> int
{
    auto const shaderProgramId = glCreateProgram();

    glAttachShader(shaderProgramId, vertexShaderId);

    glAttachShader(shaderProgramId, fragmentShaderId);

    glLinkProgram(shaderProgramId);

    checkShaderLinkingOutcome(shaderProgramId);

    glDeleteShader(vertexShaderId);

    glDeleteShader(fragmentShaderId);

    return shaderProgramId;
}

auto makeShaderProgram(
    std::string const & vertexShaderFilename,
    std::string const & fragmentShaderFilename)
    -> int
{
    auto const vertexShaderId = compileVertexShader(vertexShaderFilename);

    auto const fragmentShaderId = compileFragmentShader(fragmentShaderFilename);

    return linkShaderProgram(vertexShaderId, fragmentShaderId);
}

ShaderProgram::ShaderProgram(
    std::string const & vertexShaderFilename,
    std::string const & fragmentShaderFilename)
    : id{makeShaderProgram(vertexShaderFilename, fragmentShaderFilename)}
{
}

auto ShaderProgram::use() const
    -> void
{
    glUseProgram(id);
}

auto ShaderProgram::getUniformLocation(std::string const & name) const
    -> int
{
    auto const location = glGetUniformLocation(id, name.c_str());

    if (location < 0)
    {
        throw UniformNotFound{name};
    }

    return location;
}
