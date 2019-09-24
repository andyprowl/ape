#include "WidgetWindow.hpp"

#include <QKeyEvent>
#include <QOpenGLContext>

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

auto translateKey(Qt::Key const key)
    -> ape::Key
{
    auto const it = keyTranslationTable.find(key);

    if (it == std::cend(keyTranslationTable))
    {
        return static_cast<ape::Key>(static_cast<int>(key));
    }

    return it->second;
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

} // unnamed namespace
    
WidgetWindow::WidgetWindow(QOpenGLWidget & widget)
    : widget{&widget}
{
    widget.installEventFilter(this);

    keyStatus.resize(static_cast<std::size_t>(ape::Key::last) + 1, ape::KeyStatus::released);
}

auto WidgetWindow::getAspectRatio() const
    -> float
{
    auto const size = widget->size();

    return (static_cast<float>(size.width()) / size.height());
}

auto WidgetWindow::getCenter() const
    -> Position<double>
{
    auto const size = widget->size();

    return {size.width() / 2.0, size.height() / 2.0};
}

auto WidgetWindow::getSize() const
    -> Size<int>
{
    auto const size = widget->size();

    return {size.width(), size.height()};
}

auto WidgetWindow::getPosition() const
    -> Position<int>
{
    auto const position = widget->pos();

    return {position.x(), position.y()};
}

auto WidgetWindow::getMousePosition() const
    -> Position<double>
{
    auto const mousePosition = QCursor::pos();

    return {static_cast<float>(mousePosition.x()), static_cast<float>(mousePosition.y())};
}

auto WidgetWindow::isKeyPressed(Key const key) const
    -> bool
{
    return (keyStatus[static_cast<std::size_t>(key)] == KeyStatus::pressed);
}

auto WidgetWindow::shouldClose() const
    -> bool
{
    return false;
}

auto WidgetWindow::requestClosure()
    -> void
{
    return;
}

auto WidgetWindow::swapBuffers()
    -> void
{
    // Qt framework takes care of this
}

auto WidgetWindow::isMouseCaptured() const
    -> bool
{
    return widget->hasMouseTracking();
}

auto WidgetWindow::captureMouse()
    -> void
{
    widget->setMouseTracking(true);
}

auto WidgetWindow::releaseMouse()
    -> void
{
    widget->setMouseTracking(false);
}

auto WidgetWindow::isFullScreen() const
    -> bool
{
    return false;
}

auto WidgetWindow::setFullScreen()
    -> void
{
    return;
}

auto WidgetWindow::exitFullScreen()
    -> void
{
    return;
}

auto WidgetWindow::makeCurrent()
    -> void
{
    widget->makeCurrent();
}

auto WidgetWindow::pollEvents()
    -> void
{
    // Qt framework takes care of this
}

auto WidgetWindow::eventFilter(QObject * const obj, QEvent * const e)
    -> bool
{
    if (e->type() == QEvent::KeyPress)
    {
        return processKeyPressEvent(e);
    }
    else if (e->type() == QEvent::KeyRelease)
    {
        return processKeyReleaseEvent(e);
    }
    else
    {
        return QObject::eventFilter(obj, e);
    }
}

auto WidgetWindow::processKeyPressEvent(QEvent const * e)
    -> bool
{
    return processKeyEvent(e, ape::KeyStatus::pressed, ape::KeyAction::press);
}

auto WidgetWindow::processKeyReleaseEvent(QEvent const * e)
    -> bool
{
    return processKeyEvent(e, ape::KeyStatus::released, ape::KeyAction::release);
}

auto WidgetWindow::processKeyEvent(
    QEvent const * const e,
    ape::KeyStatus const status,
    ape::KeyAction const action)
    -> bool
{
    auto const keyEvent = static_cast<QKeyEvent const *>(e);

    auto const key = translateKey(static_cast<Qt::Key>(keyEvent->key()));

    if (key > ape::Key::last)
    {
        return false;
    }

    auto const modifiers = translateModifier(keyEvent->modifiers());

    keyStatus[static_cast<std::size_t>(key)] = status;

    onKeyboard.fire(key, action, static_cast<ape::KeyModifier>(modifiers));

    return true;
}

} // namespace ape::qt
