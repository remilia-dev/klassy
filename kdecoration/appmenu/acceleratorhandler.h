#pragma once

#include <kwin/input.h>

namespace KWin
{
class KeyboardKeyEvent;
}

class QMutex;

namespace Klassy
{
class AppMenuAcceleratorHandler : KWin::InputEventFilter
{
    AppMenuAcceleratorHandler();

    bool keyboardKey(KWin::KeyboardKeyEvent *event) override;

    static QMutex registeredMutex;
    static bool registered;

public:
    static void registerHandler();
};
}