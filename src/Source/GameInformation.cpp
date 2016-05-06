#include "./GameInformation.hpp"
#include "./ScopedHandle.hpp"

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <psapi.h>

namespace
{
    HWND findGameWindow()
    {
        return FindWindowW(L"MainWindow", L"Fantasy Earth Zero");
    }
}

juce::Rectangle<int> GameInformation::lookupGameWindowBounds() const
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


void GameInformation::updateGameWindowBounds(juce::Rectangle<int> const& bounds) const
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


File GameInformation::lookupReplaySaveFolder() const
{
    HWND handle = findGameWindow();
    if (handle == NULL) {
        return File::nonexistent;
    }

    DWORD pid = 0;
    GetWindowThreadProcessId(handle, &pid);
    if (pid == 0) {
        return File::nonexistent;
    }

    ScopedHandle process = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
                                       FALSE, // bInheritHandle
                                       pid);
    if (process == NULL) {
        return File::nonexistent;
    }

    std::vector<wchar_t> filePath(MAX_PATH, (wchar_t)0);
    DWORD const resultSize = GetModuleFileNameExW(process,
                                                  NULL, // hModule
                                                  filePath.data(), // lpFilename
                                                  filePath.size());
    if (resultSize == 0) {
        return File::nonexistent;
    }

    String path(filePath.data());
    File fezClientExecutable(path);
    if (!fezClientExecutable.existsAsFile()) {
        return File::nonexistent;
    }

    File replayFolder = fezClientExecutable.getParentDirectory().getChildFile(String(L"Replay"));

    if (replayFolder.exists() && replayFolder.isDirectory()) {
        return replayFolder;
    } else {
        return File::nonexistent;
    }
}
