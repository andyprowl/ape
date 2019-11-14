#pragma once

#include <Basix/Signal/ScopedSignalConnection.hpp>

#include <algorithm>
#include <functional>
#include <memory>
#include <vector>

namespace basix
{

/*
IMPORTANT: Signals are movable, but move-assignment will invalidate all existing connections to the
assigned-to signal. Also, connections to moved-from signals are invalid.
Invalidated ScopedConnection objects should be detached (by calling the detach() member function)
before being destroyed. Failing to do so will result in undefined behavior at destruction.
*/
template<typename S>
class Signal
{

public:

    using Cookie = int;

    using Handler = std::function<S>;

public:

    Signal()
        : nextCookie{0}
    {
    }

    Signal(Signal const & rhs) = delete;

    // IMPORTANT: See the class description. Notice: NOT noexcept.
    Signal(Signal && rhs) = default;

    auto operator = (Signal const & rhs)
        -> Signal & = delete;

    // IMPORTANT: See the class description. Notice: NOT noexcept.
    auto operator = (Signal && rhs)
        -> Signal & = default;

    ~Signal() = default;

    auto registerHandler(Handler handler)
        -> ScopedSignalConnection
    {
        auto const cookie = ++nextCookie;

        registrations.emplace_back(cookie, std::move(handler));

        return ScopedSignalConnection{[this, cookie]
        { 
            return unregisterHandler(cookie);
        }};
    }

    auto unregisterHandler(Cookie const cookie)
        -> void
    {
        for (auto it = std::cbegin(registrations); it != std::cend(registrations); ++it)
        {
            if (it->cookie == cookie)
            {
                registrations.erase(it);

                return;
            }
        }
    }

    // Notice that arguments are taken by forwarding references although no forwarding is done.
    // This allows forwarding references to both const and non-const argument without forcing one
    // or the other. It is up to handlers to behave sensibly when the arguments are taken by non-
    // const reference, because possible changes will be seen by handlers that get notified later.
    template<typename... Args>
    auto fire(Args && ... args) const
        -> void
    {
        for (auto const & registration : registrations)
        {
            registration.handler(args...);
        }
    }

private:

    class Registration
    {

    public:

        Registration(Cookie const cookie, Handler handler)
            : cookie{cookie}
            , handler{std::move(handler)}
        {
        }

    public:

        Cookie cookie;

        Handler handler;

    };

private:

    Cookie nextCookie;

    std::vector<Registration> registrations;

};

} // namespace basix
