#pragma once

#include <Ape/Engine/Engine/FrameLoop.hpp>

#include <QTimer.h>

namespace ape::qt
{

class QtEventFrameLoop : public QObject, public FrameLoop
{

    Q_OBJECT

public:

    // virtual (from FrameLoop)
    auto start(FrameProcessor processor)
        -> void override;

    // virtual (from FrameLoop)
    auto stop()
        -> void override;

private:

    QTimer heartbeat;

    QMetaObject::Connection loopTimerConnection;

};

} // namespace ape::qt
