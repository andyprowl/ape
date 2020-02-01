#pragma once

#include <Glow/Shader/Uniform.hpp>

namespace glow
{

template<typename UniformType>
class UniformArray
{

public:

    using ValueType = std::vector<typename UniformType::ValueType>;

    template<typename T>
    using EnableIfUniformType = std::enable_if_t<
        std::is_same_v<decltype(std::declval<T>().get(), 0), int>>;

public:

    UniformArray(ShaderProgram & program, std::string name)
        : program{&program}
        , name{std::move(name)}
    {
    }

    UniformArray(ShaderProgram & program, std::string name, int const size)
        : UniformArray{program, std::move(name)}
    {
        resize(size);
    }

    auto operator [] (int const i)
        -> UniformType &
    {
        if (i >= static_cast<int>(uniforms.size()))
        {
            resize(i + 1);
        }

        return uniforms[i];
    }

    auto resize(int const newSize)
        -> void
    {
        if (newSize == static_cast<int>(uniforms.size()))
        {
            return;
        }

        if (newSize > static_cast<int>(uniforms.size()))
        {
            for (auto i = static_cast<int>(uniforms.size()); i < newSize; ++i)
            {
                uniforms.emplace_back(*program, name + "[" + std::to_string(i) + "]");
            }
        }
        else
        {
            uniforms.erase(std::cbegin(uniforms) + newSize, std::cend(uniforms));
        }
    }

    template<typename T = ValueType, EnableIfUniformType<T> * = nullptr>
    auto get() const
        -> ValueType
    {
        auto values = ValueType{};

        for (auto const & uniform : uniforms)
        {
            auto value = uniform.get();

            values.push_back(std::move(value));
        }

        return values;
    }

    auto set(ValueType const & values)
        -> void
    {
        auto numOfValues = static_cast<int>(values.size());

        resize(numOfValues);

        for (auto i = 0; i < numOfValues; ++i)
        {
            uniforms[i].set(values[i]);
        }
    }

private:

    ShaderProgram * program;

    std::string name;

    std::vector<UniformType> uniforms;

};

} // namespace glow
