#include "appmenu/acceleratorhandler.h"
#include "breezedecoration.h"

// KWin
#include <kwin/focuschain.h>
#include <kwin/input.h>
#include <kwin/input_event.h>
#include <kwin/keyboard_input.h>
#include <kwin/wayland/seat.h>
#include <kwin/wayland_server.h>
#include <kwin/waylandwindow.h>
#include <kwin/window.h>
#include <kwin/workspace.h>

// Qt
#include <QMutexLocker>
#include <QObject>
#include <QTimer>
#include <qkeysequence.h>

namespace Klassy
{

AppMenuAcceleratorHandler::AppMenuAcceleratorHandler()
    : KWin::InputEventFilter(KWin::InputFilterOrder::WindowAction)
{
}

void AppMenuAcceleratorHandler::registerHandler()
{
    if (registered)
        return;
    QMutexLocker _lock(&registeredMutex);
    if (registered)
        return;
    auto input = KWin::input();
    // NOTE: A KDED6 process also uses the decorations briefly; we cannot register an input filter in that process.
    if (!input)
        return;
    input->installInputEventFilter(new AppMenuAcceleratorHandler());
    registered = true;
}

Decoration *getActiveDecoration()
{
    if (auto window = KWin::Workspace::self()->activeWindow()) {
        return qobject_cast<Decoration *>(window->decoration());
    }
    return nullptr;
}

bool AppMenuAcceleratorHandler::keyboardKey(KWin::KeyboardKeyEvent *event)
{
    if (event->key == Qt::Key_Alt) {
        if (auto decoration = getActiveDecoration()) {
            decoration->setAltHeld(event->state != KWin::KeyboardKeyState::Released);
        }
    } else if (event->modifiersRelevantForGlobalShortcuts == Qt::KeyboardModifier::AltModifier) {
        if (auto decoration = getActiveDecoration()) {
            auto key = QKeySequence(Qt::ALT | event->key);
            return decoration->handleAccelerator(key);
        }
    }

    return KWin::InputEventFilter::keyboardKey(event);
}

bool AppMenuAcceleratorHandler::registered = false;
QMutex AppMenuAcceleratorHandler::registeredMutex = QMutex();
}