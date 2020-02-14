#include <Basix/CompilerSupport/BuildConfiguration.hpp>

#define makeResolvedStringLiteral(CONFIG) #CONFIG

#define makeStringLiteral(CONFIG) makeResolvedStringLiteral(CONFIG)

namespace basix
{

auto getConfigurationName()
    -> const char *
{
    return makeStringLiteral(buildConfiguration);
}

auto getBuildLabel() 
    -> const char *
{
    return makeStringLiteral(buildLabel);
}

auto getSourceControlRevision() 
    -> const char *
{
    return makeStringLiteral(sourceControlRevision);
}

auto getSourceControlBranch()  
    -> const char *
{
    return makeStringLiteral(sourceControlBranch);
}

auto getSourceControlTag()  
    -> const char *
{
    return makeStringLiteral(sourceControlTag);
}

} // namespace basix
