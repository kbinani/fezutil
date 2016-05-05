#include "./FEZUtilApplication.hpp"
#include "./TrayIcon.hpp"
#include "./CommandIDs.hpp"
#include "./GameWindowPositionWatcher.hpp"

FEZUtilApplication::FEZUtilApplication()
    : monitoringGameWindowPosition_(true)
{
}


FEZUtilApplication::~FEZUtilApplication()
{
}


void FEZUtilApplication::initialise(String const& commandLine)
{
    commandManager_ = new ApplicationCommandManager();
    commandManager_->registerAllCommandsForTarget(this);

    trayIcon_ = new TrayIcon(this, commandManager_);
    startTimerHz(1);
}


void FEZUtilApplication::shutdown()
{
    trayIcon_ = nullptr;
    commandManager_ = nullptr;
    stopTimer();
}


void FEZUtilApplication::timerCallback()
{
    if (monitoringGameWindowPosition_) {
        bool const shouldReplace = watcher_ != nullptr && !watcher_->isWorking();

        if (watcher_ == nullptr || shouldReplace) {
            watcher_ = nullptr;
            watcher_ = new GameWindowPositionWatcher();
        }
    } else {
        watcher_ = nullptr;
    }
}


void FEZUtilApplication::getAllCommands(Array<CommandID>& commands)
{
    JUCEApplication::getAllCommands(commands);

    CommandID ids[] = {
        CommandIDs::EnableGameWindowPositionWatcher,
        CommandIDs::DisableGameWindowPositionWatcher,
    };

    commands.addArray(ids, numElementsInArray(ids));
}


void FEZUtilApplication::getCommandInfo(CommandID commandID, ApplicationCommandInfo& result)
{
    switch (commandID) {
        case CommandIDs::DisableGameWindowPositionWatcher:
        {
            int flags = 0;
            result.setInfo(TRANS("Monitor Off"), TRANS("Disable monitoring game window"), String::empty, flags);
            break;
        }
        case CommandIDs::EnableGameWindowPositionWatcher:
        {
            int flags = 0;
            result.setInfo(TRANS("Monitor On"), TRANS("Enable monitoring game window"), String::empty, flags);
            break;
        }
        default:
        {
            JUCEApplication::getCommandInfo(commandID, result);
            break;
        }
    }
}


bool FEZUtilApplication::perform(InvocationInfo const& info)
{
    switch (info.commandID) {
        case CommandIDs::DisableGameWindowPositionWatcher:
        {
            monitoringGameWindowPosition_ = false;
            return true;
        }
        case CommandIDs::EnableGameWindowPositionWatcher:
        {
            monitoringGameWindowPosition_ = true;
            return true;
        }
        default:
        {
            return JUCEApplication::perform(info);
        }
    }
}

