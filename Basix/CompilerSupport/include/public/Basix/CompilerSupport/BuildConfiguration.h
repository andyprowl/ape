#pragma once

namespace basix
{

auto getConfigurationName()
    -> const char *;

auto getBuildLabel()
    -> const char *;

auto getSourceControlRevision()
    -> const char *;

auto getSourceControlBranch()
    -> const char *;

auto getSourceControlTag()
    -> const char *;

} // namespace basix
