#pragma once

#include <Glow/Shader/UniformBlockMember.hpp>
#include <glow/Shader/UniformTypeTraits.hpp>

#include <string>
#include <string_view>
#include <vector>

namespace glow::detail
{

class BasicUniformArrayBlockMember : public BasicUniformBlockMember
{

public:

    BasicUniformArrayBlockMember(UniformBlock & block, std::string_view name);

    auto getArrayStride() const
        -> int;

private:

    auto fetchArrayStride() const
        -> int;

private:

    int arrayStride;

};

class BasicUniformMatrixArrayBlockMember : public BasicUniformArrayBlockMember
{

public:

    BasicUniformMatrixArrayBlockMember(UniformBlock & block, std::string_view name);

    auto getMatrixStride() const
        -> int;

private:

    auto fetchMatrixStride(std::string_view name) const
        -> int;
    
    auto fetchFirstElementUniformIndex(std::string_view name) const
        -> int;

private:

    int matrixStride;

};

} // namespace glow::detail

namespace glow
{

template<typename T, typename = void>
class UniformArrayBlockMember;

template<typename T>
class UniformArrayBlockMember<T, std::enable_if_t<not isFundamental<T>>>
{

public:

    using ElementValueType = T;

    using ValueType = std::vector<T>;

    using BlockMemberType = UniformBlockMember<T>;

public:

    UniformArrayBlockMember(
        UniformBlock & block,
        std::string_view const name,
        int const maxNumOfElements)
        : elements{makeElements(block, name, maxNumOfElements)}
    {
    }
    
    auto operator [] (int const index)
        -> BlockMemberType &
    {
        return elements[index];
    }

    auto get(int index, std::byte * const buffer) const
        -> ElementValueType const &
    {
        auto & element = elements[index];
        
        return element.get(buffer);
    }

    auto get(std::byte * const buffer) const
        -> ValueType
    {
        auto values = ValueType{};

        for (auto const & element : elements)
        {
            auto value = element.get(buffer);

            values.push_back(std::move(value));
        }

        return values;
    }

    auto set(ElementValueType const & value, int const index, std::byte * const buffer)
        -> void
    {
        auto & element = elements[index];

        element.set(value, buffer);
    }

    auto set(ValueType const & values, std::byte * const buffer)
        -> void
    {
        auto numOfValues = static_cast<int>(values.size());

        for (auto i = 0; i < numOfValues; ++i)
        {
            set(values[i], i, buffer);
        }
    }

private:

    auto makeElements(
        UniformBlock & block,
        std::string_view const name,
        int const maxNumOfElements) const
        -> std::vector<BlockMemberType>
    {
        auto arrayElements = std::vector<BlockMemberType>{};

        for (auto i = 0; i < maxNumOfElements; ++i)
        {
            auto const uniformName = std::string{name} + "[" + std::to_string(i) + "]";

            arrayElements.emplace_back(block, uniformName);
        }

        return arrayElements;
    }

private:

    std::vector<BlockMemberType> elements;

};

template<typename T>
class UniformArrayBlockMember<T, std::enable_if_t<isFundamental<T> && not isMatrix<T>>> 
    : public detail::BasicUniformArrayBlockMember
{

public:

    using ElementValueType = T;

    using ValueType = std::vector<T>;

public:

    UniformArrayBlockMember(
        UniformBlock & block,
        std::string_view const name,
        int const maxNumOfElements)
        : BasicUniformArrayBlockMember{block, name}
        , maxNumOfElements{maxNumOfElements}
    {
    }
    
    auto get(int const i, std::byte * const buffer) const
        -> ElementValueType const &
    {
        auto const arrayBaseOffset = getOffset();

        auto const elementStride = getArrayStride();

        auto const valueBytes = std::next(buffer, arrayBaseOffset + i * elementStride);

        return *(reinterpret_cast<ElementValueType const *>(valueBytes));
    }

    auto get(std::byte * const buffer) const
        -> ValueType
    {
        auto const arrayBaseOffset = getOffset();

        auto const elementStride = getArrayStride();

        auto values = ValueType{};

        for (auto i = 0; i < maxNumOfElements; ++i)
        {
            auto const valueBytes = std::next(buffer, arrayBaseOffset + i * elementStride);

            auto const & value = *(reinterpret_cast<ElementValueType const *>(valueBytes));

            values.push_back(value);
        }

        return values;
    }

    auto set(ElementValueType const & value, int const i, std::byte * const buffer)
        -> void
    {
        auto const arrayBaseOffset = getOffset();

        auto const elementStride = getArrayStride();

        auto const target = std::next(buffer, arrayBaseOffset + i * elementStride);

        auto const valueBytes = reinterpret_cast<std::byte const *>(&value);

        std::copy(valueBytes, std::next(valueBytes, sizeof(ElementValueType)), target);
    }

    auto set(ValueType const & values, std::byte * const buffer)
        -> void
    {
        auto const arrayBaseOffset = getOffset();

        auto const elementStride = getArrayStride();

        auto numOfValues = static_cast<int>(values.size());

        for (auto i = 0; i < numOfValues; ++i)
        {
            auto const target = std::next(buffer, arrayBaseOffset + i * elementStride);

            auto const valueBytes = reinterpret_cast<std::byte const *>(&values[i]);

            std::copy(valueBytes, std::next(valueBytes, sizeof(ElementValueType)), target);
        }
    }

private:

    int maxNumOfElements;

};

template<typename T>
class UniformArrayBlockMember<T, std::enable_if_t<isMatrix<T>>>
    : public detail::BasicUniformMatrixArrayBlockMember
{

public:

    using ElementValueType = T;

    using ValueType = std::vector<T>;

public:

    UniformArrayBlockMember(
        UniformBlock & block,
        std::string_view const name,
        int const maxNumOfElements)
        : BasicUniformMatrixArrayBlockMember{block, name}
        , maxNumOfElements{maxNumOfElements}
    {
    }

    auto get(int const i, std::byte * const buffer) const
        -> ElementValueType
    {
        auto const arrayBaseOffset = getOffset();

        auto const elementStride = getArrayStride();

        auto const matrixStride = getMatrixStride();

        auto const valueBytes = std::next(buffer, arrayBaseOffset + i * elementStride);
        
        return glow::getMatrix<ElementValueType>(valueBytes, matrixStride);
    }

    auto get(std::byte * const buffer) const
        -> ValueType
    {
        auto const arrayBaseOffset = getOffset();

        auto const elementStride = getArrayStride();

        auto const matrixStride = getMatrixStride();

        auto values = ValueType{};

        for (auto i = 0; i < maxNumOfElements; ++i)
        {
            auto const valueBytes = std::next(buffer, arrayBaseOffset + i * elementStride);

            auto const value = glow::getMatrix<ElementValueType>(valueBytes, matrixStride);

            values.push_back(value);
        }

        return values;
    }

    auto set(ElementValueType const & value, int const i, std::byte * const buffer)
        -> void
    {
        auto const arrayBaseOffset = getOffset();

        auto const elementStride = getArrayStride();

        auto const columnStride = getMatrixStride();

        auto const target = std::next(buffer, arrayBaseOffset + i * elementStride);

        glow::setMatrix(value, target, columnStride);
    }

    auto set(ValueType const & values, std::byte * const buffer)
        -> void
    {
        auto const arrayBaseOffset = getOffset();

        auto const elementStride = getArrayStride();

        auto const matrixStride = getMatrixStride();

        auto numOfValues = static_cast<int>(values.size());

        for (auto i = 0; i < numOfValues; ++i)
        {
            auto const target = std::next(buffer, arrayBaseOffset + i * elementStride);

            glow::setMatrix(values[i], target, matrixStride);
        }
    }

private:

    int maxNumOfElements;

};

} // namespace glow
