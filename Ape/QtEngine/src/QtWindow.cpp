#include <Ape/QtEngine/QtWindow.hpp>

#include <Ape/Initialization/Initialization.hpp>
#include <Ape/UpdateHandling/StandardInputHandler.hpp>
#include <Ape/Windowing/Keyboard.hpp>

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

QtWindow::QtWindow(QWidget * const parent)
    : QOpenGLWidget{parent}
    , isMouseGrabbed{false}
    , isWindowClosing{false}
    , isOpenGLInitialized{false}
{
    keyStatus.resize(static_cast<std::size_t>(ape::Key::last) + 1, ape::KeyStatus::released);
}

QtWindow::~QtWindow()
{
    makeCurrent();

    // Do all resource releasing here

    doneCurrent();
}

// virtual (from Window)
auto QtWindow::getAspectRatio() const
    -> float
{
    auto const windowSize = size();

    return (static_cast<float>(windowSize.width()) / windowSize.height());
}

// virtual (from Window)
auto QtWindow::getSize() const
    -> Size<int>
{
    auto const windowSize = size();

    return {windowSize.width(), windowSize.height()};
}

// virtual (from Window)
auto QtWindow::getPosition() const
    -> Position<int>
{
    auto const position = pos();

    return {position.x(), position.y()};
}

// virtual (from Window)
auto QtWindow::getMousePosition() const
    -> Position<int>
{
    auto const mouseScreenPosition = QCursor::pos();
    
    auto const mousePosition = mapFromGlobal(mouseScreenPosition);

    return {mousePosition.x(), mousePosition.y()};
}

// virtual (from Window)
auto QtWindow::isKeyPressed(Key const key) const
    -> bool
{
    return (keyStatus[static_cast<std::size_t>(key)] == KeyStatus::pressed);
}

// virtual (from Window)
auto QtWindow::isFullScreen() const
    -> bool
{
    return (windowState() & Qt::WindowFullScreen);
}

// virtual (from Window)
auto QtWindow::setFullScreen()
    -> void
{
    setWindowState(windowState() | Qt::WindowFullScreen);
}

// virtual (from Window)
auto QtWindow::exitFullScreen()
    -> void
{
    setWindowState(windowState() & ~Qt::WindowFullScreen);
}

// virtual (from Window)
auto QtWindow::isMouseCaptured() const
    -> bool
{
    return isMouseGrabbed;
}

// virtual (from Window)
auto QtWindow::captureMouse()
    -> void
{
    QOpenGLWidget::grabMouse();

    isMouseGrabbed = true;
}

// virtual (from Window)
auto QtWindow::releaseMouse()
    -> void
{
    QOpenGLWidget::releaseMouse();
    
    isMouseGrabbed = false;
}

// virtual (from Window)
auto QtWindow::swapBuffers()
    -> void
{
    QOpenGLWidget::update();
}

// virtual (from Window)
auto QtWindow::makeCurrent()
    -> void
{
    return QOpenGLWidget::makeCurrent();
}

// virtual (from Window)
auto QtWindow::close()
    -> void
{
    QOpenGLWidget::close();
}

// virtual (from Window)
auto QtWindow::isClosing()
    -> bool
{
    return isWindowClosing;
}

// virtual (from QOpenGLWidget)
auto QtWindow::initializeGL()
    -> void
{
    initializeOpenGLFunctions();

    isOpenGLInitialized = true;

    ape::initialize();
}

// virtual (from QOpenGLWidget)
auto QtWindow::paintEvent(QPaintEvent *)
    -> void
{
}

// virtual (from QOpenGLWidget)
auto QtWindow::focusInEvent(QFocusEvent * const)
    -> void
{
    setMouseTracking(true);

    onFocusAcquired.fire();
}

// virtual (from QOpenGLWidget)
auto QtWindow::focusOutEvent(QFocusEvent * const)
    -> void
{
    onFocusLost.fire();
}

// virtual (from QOpenGLWidget)
auto QtWindow::resizeEvent(QResizeEvent * const e)
    -> void
{
    if (!isOpenGLInitialized)
    {
        return;
    }

    auto const newSize = e->size();

    onResize.fire(Size<int>{newSize.width(), newSize.height()});
}

// virtual (from QOpenGLWidget)
auto QtWindow::keyPressEvent(QKeyEvent * const e)
    -> void
{
    handleKeyEvent(*e, KeyStatus::pressed, [this] (Key const key, KeyModifier const modifier)
    {
        onKeyboard.fire(key, KeyAction::press, modifier);
    });
}

// virtual (from QOpenGLWidget)
auto QtWindow::keyReleaseEvent(QKeyEvent * const e)
    -> void
{
    handleKeyEvent(*e, KeyStatus::released, [this] (Key const key, KeyModifier const modifier)
    {
        onKeyboard.fire(key, KeyAction::release, modifier);
    });
}

// virtual (from QOpenGLWidget)
auto QtWindow::wheelEvent(QWheelEvent * const e)
    -> void
{
    auto const delta = e->angleDelta();

    auto const deltaX = delta.x() > 0 ? 1 : -1;

    auto const deltaY = delta.y() > 0 ? 1 : -1;

    onMouseWheel.fire(Offset<int>{deltaX, deltaY});
}

// virtual (from QOpenGLWidget)
auto QtWindow::closeEvent(QCloseEvent * const e)
    -> void
{
    isWindowClosing = true;

    e->accept();
}

template<typename F>
auto QtWindow::handleKeyEvent(QKeyEvent & e, KeyStatus status, F inputHandlerNotifier)
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

auto QtWindow::setKeyStatus(Key const key, KeyStatus const status)
    -> void
{
    keyStatus[static_cast<std::size_t>(key)] = status;
}

} // namespace ape::qt
