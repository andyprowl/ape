#pragma once

#include <Signal/ScopedSignalConnection.hpp>

#include <functional>
#include <unordered_map>

namespace ape
{

template<typename S>
class Signal
{

public:

    using Cookie = int;

    using Handler = std::function<S>;

public:

    auto registerHandler(Handler handler)
        -> ScopedSignalConnection
    {
        auto const cookie = static_cast<int>(registrations.size());

        registrations.emplace(cookie, std::move(handler));

        return ScopedSignalConnection{[=]
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
