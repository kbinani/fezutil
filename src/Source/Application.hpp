#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class TrayIcon;
class GameWindowPositionWatcher;
class ReplaySaveFolderWatcher;

class Application
    : public JUCEApplication
    , public Timer
{
public:
    Application();

    ~Application();

    bool isMonitoringGameWindow() const
    {
        return monitoringGameWindowPosition_;
    }

private:
    const String getApplicationName() override
    {
        return ProjectInfo::projectName;
    }

    const String getApplicationVersion() override
    {
        return ProjectInfo::versionString;
    }

    bool moreThanOneInstanceAllowed() override
    {
        return true;
    }

    void initialise(String const& commandLine) override;

    void shutdown() override;

    void systemRequestedQuit() override
    {
        quit();
    }

    void anotherInstanceStarted(String const& commandLine) override
    {
    }

    void timerCallback() override;

    void getAllCommands(Array<CommandID>& commands) override;

    void getCommandInfo(CommandID commandID, ApplicationCommandInfo& result) override;

    bool perform(InvocationInfo const& info) override;

private:
    ScopedPointer<TrayIcon> trayIcon_;
    ScopedPointer<ApplicationCommandManager> commandManager_;
    ScopedPointer<GameWindowPositionWatcher> gameWindowPositionWatcher_;
    bool monitoringGameWindowPosition_;
};
