#pragma once

template<typename Bindable>
class ScopedBinder
{

public:

    ScopedBinder()
        : bindable{nullptr}
    {
    }

    explicit ScopedBinder(Bindable & bindable)
        : bindable{&bindable}
    {
        bindable.bind();
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
