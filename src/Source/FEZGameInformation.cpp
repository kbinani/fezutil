#include "./FEZGameInformation.hpp"
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace
{
    HWND findGameWindow()
    {
        return FindWindowW(L"MainWindow", L"Fantasy Earth Zero");
    }
}

juce::Rectangle<int> FEZGameInformation::lookupGameWindowBounds() const
{
    juce::Rectangle<int> bounds;
    HWND handle = findGameWindow();

    if (handle == NULL) {
        return bounds;
    }

    RECT rect;
    if (!GetWindowRect(handle, &rect)) {
        return bounds;
    }

    bounds.setPosition(rect.left, rect.top);
    bounds.setSize(rect.right - rect.left, rect.bottom - rect.top);

    return bounds;
}


void FEZGameInformation::updateGameWindowBounds(juce::Rectangle<int> const& bounds) const
{
    HWND handle = findGameWindow();
    if (handle == NULL) {
        return;
    }

    MoveWindow(handle,
               bounds.getX(), bounds.getY(),
               bounds.getWidth(), bounds.getHeight(),
               FALSE); // bRepaint
}
