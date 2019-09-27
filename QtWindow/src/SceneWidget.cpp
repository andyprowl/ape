#include <QtWindow/SceneWidget.hpp>

#include <Ape/CameraSelector.hpp>
#include <Ape/Keyboard.hpp>
#include <Ape/SceneRenderer.hpp>
#include <Ape/StandardInputHandler.hpp>
#include <Ape/StandardShaderProgram.hpp>

#include <QResizeEvent>
#include <QTimer>

#include <unordered_map>

namespace ape::qt
{

namespace
{

auto const keyTranslationTable = std::unordered_map<Qt::Key, ape::Key>{
    {Qt::Key_Up, ape::Key::keyUp},
    {Qt::Key_Down, ape::Key::keyDown},
    {Qt::Key_Left, ape::Key::keyLeft},
    {Qt::Key_Right, ape::Key::keyRight},
    {Qt::Key_Shift, ape::Key::keyLeftShift},
    {Qt::Key_Control, ape::Key::keyLeftControl}};

auto const modifierTranslationTable = std::unordered_map<Qt::KeyboardModifier, ape::KeyModifier>{
    {Qt::KeyboardModifier::ShiftModifier, ape::KeyModifier::shift},
    {Qt::KeyboardModifier::ControlModifier, ape::KeyModifier::control},
    {Qt::KeyboardModifier::AltModifier, ape::KeyModifier::alt},
    {Qt::KeyboardModifier::NoModifier, ape::KeyModifier::none}};

auto unshift(QKeyEvent & e)
    -> Key
{
    if ((e.nativeVirtualKey() >= 0x30) && (e.nativeVirtualKey() <= 0x39))
    {
        return static_cast<Key>(static_cast<int>(Key::key0) + (e.nativeVirtualKey() - 0x30));
    }
    else
    {
        return static_cast<Key>(e.key());
    }
}

auto translateKey(QKeyEvent & e)
    -> ape::Key
{
    auto const key = static_cast<Qt::Key>(e.key());

    auto const it = keyTranslationTable.find(key);

    if (it != std::cend(keyTranslationTable))
    {
        return it->second;
    }

    if (e.modifiers() & Qt::KeyboardModifier::ShiftModifier)
    {
        return unshift(e);
    }

    return static_cast<ape::Key>(static_cast<int>(key));
}

auto translateModifier(Qt::KeyboardModifiers const modifier)
    -> ape::KeyModifier
{
    auto translatedModifiers = 0;

    for (auto const & entry : modifierTranslationTable)
    {
        if (modifier & entry.first)
        {
            translatedModifiers |= static_cast<int>(entry.second);
        }
    }

    return static_cast<ape::KeyModifier>(translatedModifiers);
}

auto getKey(QKeyEvent & e)
    -> std::optional<Key>
{
    auto const key = translateKey(e);

    if (key > ape::Key::last)
    {
        return std::nullopt;
    }

    return key;
}

} // unnamed namespace

class SceneWidget::EngineHolder
{

public:

    EngineHolder(
        CameraSelector & cameraSelector,
        InputHandler & inputHandler,
        StandardShaderProgram & shader,
        RenderingContext const & context)
        : cameraSelector{&cameraSelector}
        , inputHandler{&inputHandler}
        , renderer{context, cameraSelector, shader, {0.0f, 0.0f, 0.0f}}
    {
    }

public:

    CameraSelector * cameraSelector;

    InputHandler * inputHandler;

    SceneRenderer renderer;

};

SceneWidget::SceneWidget(ape::RenderingContext const & renderingContext, QWidget * const parent)
    : QOpenGLWidget{parent}
    , renderingContext{renderingContext}
    , timeTracker{stopwatch}
{
    keyStatus.resize(static_cast<std::size_t>(ape::Key::last) + 1, ape::KeyStatus::released);
}

SceneWidget::~SceneWidget()
{
    makeCurrent();

    // Do all resource releasing here

    doneCurrent();
}

auto SceneWidget::engage(
    CameraSelector & cameraSelector,
    InputHandler & inputHandler,
    StandardShaderProgram & shader)
    -> void
{
    makeCurrent();

    holder = std::make_shared<EngineHolder>(cameraSelector, inputHandler, shader, renderingContext);
}

auto SceneWidget::getCameraSelector()
    -> CameraSelector &
{
    if (holder == nullptr)
    {
        throw SceneWidgetNotEngaged{};
    }

    return *(holder->cameraSelector);
}

auto SceneWidget::startEventLoop()
    -> void
{
    auto const timer = new QTimer{this};
    
    connect(timer, &QTimer::timeout, [this] 
    { 
        processOneFrame();
    });
    
    // The timer's callback will be invoke whenever the application is idle.
    timer->start();
}

auto SceneWidget::processOneFrame()
    -> void
{
    if (holder == nullptr)
    {
        return;
    }

    auto const lastFrameDuration = timeTracker.tick();

    holder->inputHandler->onFrame(lastFrameDuration);

    update();
}

// virtual (from QOpenGLWidget)
auto SceneWidget::initializeGL()
    -> void
{
    initializeOpenGLFunctions();

    startEventLoop();
}

// virtual (from QOpenGLWidget)
auto SceneWidget::paintGL()
    -> void
{
    if (holder == nullptr)
    {
        return;
    }
    
    holder->renderer.render();
}

// virtual (from QOpenGLWidget)
auto SceneWidget::focusInEvent(QFocusEvent * const)
    -> void
{
    setMouseTracking(true);

    holder->inputHandler->onFocusAcquired();
}

// virtual (from QOpenGLWidget)
auto SceneWidget::focusOutEvent(QFocusEvent * const)
    -> void
{
    holder->inputHandler->onFocusLost();
}

// virtual (from QOpenGLWidget)
auto SceneWidget::resizeEvent(QResizeEvent * const e)
    -> void
{
    if (holder == nullptr)
    {
        return;
    }

    auto const newSize = e->size();

    glViewport(0, 0, newSize.width(), newSize.height());
}

// virtual (from QOpenGLWidget)
auto SceneWidget::keyPressEvent(QKeyEvent * const e)
    -> void
{
    handleKeyEvent(*e, KeyStatus::pressed, [this] (Key const key, KeyModifier const modifier)
    {
        holder->inputHandler->onKeyPress(key, modifier);
    });
}

// virtual (from QOpenGLWidget)
auto SceneWidget::keyReleaseEvent(QKeyEvent * const e)
    -> void
{
    handleKeyEvent(*e, KeyStatus::released, [this] (Key const key, KeyModifier const modifier)
    {
        holder->inputHandler->onKeyRelease(key, modifier);
    });
}

// virtual (from QOpenGLWidget)
auto SceneWidget::mouseMoveEvent(QMouseEvent * const e)
    -> void
{
    auto const mousePosition = e->pos();

    holder->inputHandler->onMouseMove({mousePosition.x(), mousePosition.y()});
}

// virtual (from QOpenGLWidget)
auto SceneWidget::wheelEvent(QWheelEvent * const e)
    -> void
{
    auto const delta = e->delta();

    holder->inputHandler->onMouseWheel({0, delta});
}

// virtual (from Window)
auto SceneWidget::getAspectRatio() const
    -> float
{
    auto const windowSize = size();

    return (static_cast<float>(windowSize.width()) / windowSize.height());
}

// virtual (from Window)
auto SceneWidget::getSize() const
    -> Size<int>
{
    auto const windowSize = size();

    return {windowSize.width(), windowSize.height()};
}

// virtual (from Window)
auto SceneWidget::getPosition() const
    -> Position<int>
{
    auto const position = pos();

    return {position.x(), position.y()};
}

// virtual (from Window)
auto SceneWidget::getMousePosition() const
    -> Position<int>
{
    auto const mouseScreenPosition = QCursor::pos();
    
    auto const mousePosition = mapFromGlobal(mouseScreenPosition);

    return {mousePosition.x(), mousePosition.y()};
}

// virtual (from Window)
auto SceneWidget::isFullScreen() const
    -> bool
{
    return (windowState() & Qt::WindowFullScreen);
}

// virtual (from Window)
auto SceneWidget::setFullScreen()
    -> void
{
    setWindowState(windowState() | Qt::WindowFullScreen);
}

// virtual (from Window)
auto SceneWidget::exitFullScreen()
    -> void
{
    setWindowState(windowState() & ~Qt::WindowFullScreen);
}

auto SceneWidget::isKeyPressed(Key const key) const
    -> bool
{
    return (keyStatus[static_cast<std::size_t>(key)] == KeyStatus::pressed);
}

template<typename F>
auto SceneWidget::handleKeyEvent(QKeyEvent & e, KeyStatus status, F inputHandlerNotifier)
    -> void
{
    auto const key = getKey(e);

    if (!key)
    {
        return;
    }
    
    setKeyStatus(*key, status);

    auto const modifiers = translateModifier(e.modifiers());

    inputHandlerNotifier(*key, modifiers);
}

auto SceneWidget::setKeyStatus(Key const key, KeyStatus const status)
    -> void
{
    keyStatus[static_cast<std::size_t>(key)] = status;
}

} // namespace ape::qt
