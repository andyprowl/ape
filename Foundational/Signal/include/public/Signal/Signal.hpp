#pragma once

#include <Signal/ScopedSignalConnection.hpp>

#include <functional>
#include <memory>
#include <unordered_map>

namespace ape
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

    Signal() = default;

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
        auto const cookie = static_cast<int>(registrations.size());

        registrations.emplace(cookie, std::move(handler));

        return ScopedSignalConnection{[this, cookie]
        { 
            return unregisterHandler(cookie);
        }};
    }

    auto unregisterHandler(Cookie cookie)
        -> void
    {
        registrations.erase(cookie);
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
            registration.second(args...);
        }
    }

private:

    std::unordered_map<Cookie, Handler> registrations;

};

} // namespace ape
