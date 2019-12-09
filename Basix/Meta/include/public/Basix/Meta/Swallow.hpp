#pragma once

namespace basix
{

class Swallow
{

public:

    template<typename... Args>
    explicit Swallow(Args const & ...)
    {
    }

};

} // namespace basix
