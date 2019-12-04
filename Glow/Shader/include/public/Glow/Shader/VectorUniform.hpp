#pragma once

#include <Glow/Shader/Uniform.hpp>

namespace glow::detail
{

template<typename T, typename Derived>
class VectorUniform
{

public:

    using ValueType = std::vector<typename T::ValueType>;

    template<typename T>
    using EnableIfGettable = std::enable_if_t<
        std::is_same_v<decltype(std::declval<T>().get(), 0), int>>;

public:

    VectorUniform(ShaderProgram & program, std::string prefix)
        : program{&program}
        , prefix{std::move(prefix)}
    {
    }

    VectorUniform(ShaderProgram & program, std::string prefix, int const size)
        : VectorUniform{program, std::move(prefix)}
    {
        resize(size);
    }

    auto operator [] (int const i)
        -> T &
    {
        if (i >= static_cast<int>(uniforms.size()))
        {
            static_cast<Derived *>(this)->resize(i + 1);
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
                uniforms.emplace_back(*program, prefix + "[" + std::to_string(i) + "]");
            }
        }
        else
        {
            uniforms.erase(std::cbegin(uniforms) + newSize, std::cend(uniforms));
        }

        static_cast<Derived *>(this)->onSizeChanged(newSize);
    }

    template<typename T = ValueType, EnableIfGettable<T> * = nullptr>
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
            uniforms[i] = values[i];
        }
    }

    auto operator = (ValueType const & values)
        -> Derived &
    {
        set(values);

        return static_cast<Derived &>(*this);
    }

private:

    ShaderProgram * program;

    std::string prefix;

    std::vector<T> uniforms;

};

} // namespace glow::detail

namespace glow
{

template<typename T>
class SizedVectorUniform : public detail::VectorUniform<T, SizedVectorUniform<T>>
{

public:

    using BaseUniform = detail::VectorUniform<T, SizedVectorUniform<T>>;

public:

    SizedVectorUniform(ShaderProgram & program, std::string prefix)
        : BaseUniform{program, prefix}
        , arraySize{program, std::move(prefix) + "ArraySize"}
    {
    }

    SizedVectorUniform(ShaderProgram & program, std::string prefix, int const size)
        : SizedVectorUniform{program, std::move(prefix)}
    {
        BaseUniform::resize(size);
    }

    using BaseUniform::operator =;

private:

    friend class BaseUniform;

private:

    auto onSizeChanged(int const newSize)
        -> void
    {
        arraySize = newSize;
    }

private:

    Uniform<int> arraySize;

};

template<typename T>
class UnsizedVectorUniform : public detail::VectorUniform<T, UnsizedVectorUniform<T>>
{

public:

    using BaseUniform = detail::VectorUniform<T, UnsizedVectorUniform<T>>;

public:

    using BaseUniform::BaseUniform;

    using BaseUniform::operator =;

private:

    friend class BaseUniform;

private:

    auto onSizeChanged(int const /*newSize*/)
        -> void
    {
    }

};

} // namespace glow
