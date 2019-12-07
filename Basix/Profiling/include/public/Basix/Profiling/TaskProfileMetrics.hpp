#pragma once

#include <memory>

namespace basix
{

class TaskProfileMetrics
{

public:

    virtual ~TaskProfileMetrics() = default;

    virtual auto clone() const
        -> std::unique_ptr<TaskProfileMetrics> = 0;

};

} // namespace basix
