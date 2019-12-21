#include <Ape/Engine/QtEngine/QtEventFrameLoop.hpp>

namespace ape::qt
{

auto QtEventFrameLoop::start(FrameProcessor const processor)
    -> void
{
    loopTimerConnection = connect(&heartbeat, &QTimer::timeout, [=] 
    {
        processor();
    });
    
    // The timer's callback will be invoked whenever the application is idle.
    heartbeat.start();
}

auto QtEventFrameLoop::stop()
    -> void
{
    heartbeat.stop();
}

} // namepsace ape::qt
