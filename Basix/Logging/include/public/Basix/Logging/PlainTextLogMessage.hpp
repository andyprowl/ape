#pragma once

#include <Basix/Logging/LogEntry.hpp>

#include <string>

namespace basix
{

class PlainTextLogMessage : public LogEntry
{

public:

    PlainTextLogMessage(std::string message, Severity const severity)
        : LogEntry{severity}
        , message{std::move(message)}
    {
    }

    auto getMessage() const
        -> std::string const &
    {
        return message;
    }

private:

    std::string message;

};

} // namespace basix
