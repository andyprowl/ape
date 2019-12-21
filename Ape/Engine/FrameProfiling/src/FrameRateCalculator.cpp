#include <Ape/Engine/FrameProfiling/FrameRateCalculator.hpp>

namespace ape
{

namespace
{

auto getCpuTime(basix::ProfiledTask const & frameProfile)
    -> basix::CpuTimeMetrics::Nanoseconds
{
    auto const & metrics = frameProfile.getMetrics();

    auto const & cpuTimeMetrics = static_cast<basix::CpuTimeMetrics const &>(*metrics[0]);

    return cpuTimeMetrics.duration;
}

} // unnamed namespace

// This function must be invoked every time a new frame profile is added to the frame profile
// buffer in order for calculations to give correct results.
FrameRateCalculator::FrameRateCalculator(FrameProfileBuffer const & profiles, int maxNumOfSamples)
    : profiles{&profiles}
    , maxNumOfSamples{maxNumOfSamples}
    , numOfSamples{0}
    , cumulativeDuration{0}
{
}

auto FrameRateCalculator::update()
    -> void
{
    if (profiles->empty())
    {
        return;
    }

    cumulativeDuration += getCpuTime(profiles->back());

    if (numOfSamples < maxNumOfSamples)
    {
        ++numOfSamples;
    }
    else
    {
        auto const it = std::prev(profiles->end(), static_cast<std::size_t>(numOfSamples) + 1u);

        cumulativeDuration -= getCpuTime(*it);
    }
}

auto FrameRateCalculator::getAverageFrameDurationInMs() const
    -> float
{
    if (numOfSamples == 0)
    {
        return 0.0f;
    }

    return (cumulativeDuration.count() / (numOfSamples * 1'000'000.0f));
}

auto FrameRateCalculator::getAverageFrameRate() const
    -> float
{
    if (numOfSamples == 0)
    {
        return 0.0f;
    }
    
    return 1.0f / getAverageFrameDurationInMs();
}

} // namespace ape
