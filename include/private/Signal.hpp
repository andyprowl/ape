#pragma once

#include <functional>
#include <unordered_map>

template<typename S>
class Signal
{

public:

    using Cookie = int;

    using Handler = std::function<S>;

public:

    auto registerHandler(Handler handler)
        -> Cookie
    {
        auto const cookie = static_cast<int>(registrations.size());

        registrations.emplace(cookie, std::move(handler));

        return cookie;
    }

    auto unregisterHandler(Cookie cookie)
        -> void
    {
        registrations.erase(cookie);
    }

    template<typename... Args>
    auto fire(Args const & ... args) const
        -> void
    {
        for (const auto & registration : registrations)
        {
            registration.second(args...);
        }
    }

private:

    std::unordered_map<Cookie, Handler> registrations;

};