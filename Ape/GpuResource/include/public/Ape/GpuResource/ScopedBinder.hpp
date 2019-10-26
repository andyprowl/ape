#pragma once

#include <type_traits>
#include <tuple>

namespace ape
{

template<typename Bindable>
class [[nodiscard]] ScopedBinder
{

public:

    ScopedBinder()
        : bindable{nullptr}
    {
    }

    ScopedBinder(ScopedBinder const & rhs) = delete;

    ScopedBinder(ScopedBinder && rhs) noexcept
        : bindable{rhs.bindable}
    {
        rhs.bindable = nullptr;
    }

    auto operator = (ScopedBinder const & rhs)
        -> ScopedBinder & = delete;
    
    auto operator = (ScopedBinder && rhs) noexcept
        -> ScopedBinder &
    {
        unbind();

        bindable = rhs.bindable;

        rhs.bindable = nullptr;

        return *this;
    }

    ~ScopedBinder()
    {
        unbind();
    }

private:

    // This factory function is to prevent creating a temporary ScopedBinder that does not get
    // bound to a variable and immediately dies. The ScopedBinder class's [[noreturn]] attribute
    // guarantees that the return value cannot be ignored.
    template<typename Resource>
    friend auto bind(Resource & resource)
        -> ScopedBinder<Resource>;

    explicit ScopedBinder(Bindable & bindable)
        : bindable{&bindable}
    {
        bindable.bind();
    }

    auto unbind()
        -> void
    {
        if (bindable != nullptr)
        {
            bindable->unbind();
        }
    }

private:

    Bindable * bindable;

};

// This factory function is to prevent creating a temporary ScopedBinder that does not get bound to
// a variable and immediately dies. The ScopedBinder class's [[noreturn]] attribute guarantees that
// the return value cannot be ignored.
template<typename Resource>
auto bind(Resource & resource)
    -> ScopedBinder<Resource>
{
    return ScopedBinder{resource};
}

template<typename... Resources>
auto bind(Resources & ... resources)
    -> std::tuple<ScopedBinder<Resources>...>
{
    return std::make_tuple(bind(resources)...);
}

namespace detail
{

template<typename... Args, int... Is>
auto with(std::integer_sequence<int, Is...>, Args && ... args)
    -> void
{
    auto t = std::forward_as_tuple(args...);

    auto f = std::get<sizeof...(Args) - 1>(t);

    auto const binder = bind(std::get<Is>(t)...);

    f();
}

} // namespace detail

template<typename... Args>
auto with(Args && ... args)
    -> void
{
    detail::with(
        std::make_integer_sequence<int, sizeof...(Args) - 1>{},
        std::forward<Args>(args)...);
}

} // namespace ape
