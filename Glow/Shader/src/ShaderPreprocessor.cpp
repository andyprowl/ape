#include <Glow/Shader/ShaderPreprocessor.hpp>

#include <Glow/Shader/ShaderReader.hpp>

namespace glow
{

namespace
{

auto const includeDirective = std::string{"#include"};

auto extractIncludedFileName(std::string_view content, std::size_t const directiveStartIndex)
    -> std::string_view
{
    auto const includeKeywordEnd = directiveStartIndex + includeDirective.length();

    auto const fileNameStart = content.find('\"', includeKeywordEnd) + 1;

    auto const fileNameEnd = content.find('\"', fileNameStart + 1);

    return content.substr(fileNameStart, fileNameEnd - fileNameStart);
}

} // unnamed namespace

ShaderPreprocessor::ShaderPreprocessor(ShaderReader & reader)
    : reader{&reader}
{
}

auto ShaderPreprocessor::preprocess(std::string source) const
    -> std::string
{
    auto includeStart = findIncludeDirectiveIndex(source, 0);

    while (includeStart != std::string::npos)
    {
        auto const fileName = extractIncludedFileName(source, includeStart);

        auto includedContent = reader->read(fileName);
            
        auto preprocessedContent = preprocess(std::move(includedContent));

        auto const preprocessedContentLength = preprocessedContent.length();

        auto const includeEnd = source.find('\n', includeStart + fileName.length() + 3);

        auto prologue = source.substr(0, includeStart);

        auto epilogue = source.substr(includeEnd + 1);

        source = std::move(prologue) + std::move(preprocessedContent) + std::move(epilogue);

        includeStart = findIncludeDirectiveIndex(source, includeStart + preprocessedContentLength);
    }

    return source;
}

auto ShaderPreprocessor::getReader() const
    -> ShaderReader &
{
    return *reader;
}

auto ShaderPreprocessor::findIncludeDirectiveIndex(
    std::string_view const source,
    std::size_t const startIndex) const
    -> std::size_t
{
    const auto index = source.find(includeDirective, startIndex);

    if ((index != std::string::npos) && (index > 0) && (source[index - 1] != '\n'))
    {
        throw IncludeDirectiveNotAtLineStart{};
    }

    return index;
}

auto preprocessShader(ShaderPreprocessor const & preprocessor, std::filesystem::path const & path)
    -> std::string
{
    auto const & reader = preprocessor.getReader();

    auto const source = reader.read(path);

    return preprocessor.preprocess(source);
}

} // namespace glow
