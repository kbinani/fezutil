/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "./TrayIcon.hpp"
#include "./GameWindowPositionWatcher.hpp"
#include <cassert>

//==============================================================================
class FEZUtilApplication
    : public JUCEApplication
    , public Timer
{
public:
    //==============================================================================
    FEZUtilApplication() {}

    const String getApplicationName() override       { return ProjectInfo::projectName; }
    const String getApplicationVersion() override    { return ProjectInfo::versionString; }
    bool moreThanOneInstanceAllowed() override       { return true; }

    //==============================================================================
    void initialise (const String& commandLine) override
    {
        commandManager_ = new ApplicationCommandManager();
        commandManager_->registerAllCommandsForTarget(this);

        trayIcon_ = new TrayIcon(commandManager_);
        startTimerHz(1);
    }

    void shutdown() override
    {
        trayIcon_ = nullptr;
        commandManager_ = nullptr;
        stopTimer();
    }

    //==============================================================================
    void systemRequestedQuit() override
    {
        // This is called when the app is being asked to quit: you can ignore this
        // request and let the app carry on running, or call quit() to allow the app to close.
        quit();
    }

    void anotherInstanceStarted (const String& commandLine) override
    {
        // When another instance of the app is launched while this one is running,
        // this method is invoked, and the commandLine parameter tells you what
        // the other instance's command-line arguments were.
    }

    void timerCallback() override
    {
        bool const shouldReplace = watcher_ != nullptr && !watcher_->isWorking();

        if (watcher_ == nullptr || shouldReplace) {
            watcher_ = nullptr;
            watcher_ = new GameWindowPositionWatcher();
        }
    }

private:
    ScopedPointer<TrayIcon> trayIcon_;
    ScopedPointer<ApplicationCommandManager> commandManager_;
    ScopedPointer<GameWindowPositionWatcher> watcher_;
};

//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION(FEZUtilApplication)
