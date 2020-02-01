#pragma once

#include <Glow/Shader/UniformBlockMember.hpp>

#include <string>
#include <vector>

namespace glow::detail
{

class BasicUniformArrayBlockMember : public BasicUniformBlockMember
{

public:

    BasicUniformArrayBlockMember(UniformBlock & block, std::string_view name);

    auto getStride() const
        -> int;

    auto getMaxNumOfElements() const
        -> int;

private:

    auto fetchArrayStride() const
        -> int;

    auto fetchMaxNumOfElements() const
        -> int;

private:

    int stride;

    int maxNumOfElements;

};

} // namespace glow::detail

namespace glow
{

template<typename BMT>
class UniformArrayBlockMember : public detail::BasicUniformArrayBlockMember
{

public:

    using BlockMemberType = BMT;

    using ValueType = std::vector<typename BlockMemberType::ValueType>;

public:

    UniformArrayBlockMember(UniformBlock & block, std::string_view const name)
        : BasicUniformArrayBlockMember{block, name}
        , elements{makeElements(name)}
    {
    }
    
    auto operator [] (int const i)
        -> BlockMemberType &
    {
        return elements[i];
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

    auto set(ValueType const & values, std::byte * const buffer)
        -> void
    {
        auto numOfValues = static_cast<int>(values.size());

        for (auto i = 0; i < numOfValues; ++i)
        {
            elements[i].set(values[i], buffer);
        }
    }

private:

    auto makeElements(std::string_view const name) const
        -> void
    {
        auto arrayElements = std::vector<BlockMemberType>{};

        auto const size = getMaxNumOfElements();

        auto & block = getBlock();

        for (auto i = 0; i < size; ++i)
        {
            arrayElements.emplace_back(block, std::string{name} + "[" + std::to_string(i) + "]");
        }

        return arrayElements;
    }

private:

    std::vector<BlockMemberType> elements;

};

} // namespace glow
