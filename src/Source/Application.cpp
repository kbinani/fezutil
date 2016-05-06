#include "./Application.hpp"
#include "./TrayIcon.hpp"
#include "./CommandIDs.hpp"
#include "./GameWindowPositionWatcher.hpp"
#include "./ReplaySaveFolderWatcher.hpp"

Application::Application()
    : monitoringGameWindowPosition_(true)
{
}


Application::~Application()
{
}


void Application::initialise(String const& commandLine)
{
    commandManager_ = new ApplicationCommandManager();
    commandManager_->registerAllCommandsForTarget(this);

    trayIcon_ = new TrayIcon(this, commandManager_);
    startTimerHz(1);
}


void Application::shutdown()
{
    trayIcon_ = nullptr;
    commandManager_ = nullptr;
    stopTimer();
}


void Application::timerCallback()
{
    if (monitoringGameWindowPosition_) {
        bool const shouldReplace = gameWindowPositionWatcher_ != nullptr && !gameWindowPositionWatcher_->isWorking();

        if (gameWindowPositionWatcher_ == nullptr || shouldReplace) {
            gameWindowPositionWatcher_ = nullptr;
            gameWindowPositionWatcher_ = new GameWindowPositionWatcher();
        }
    } else {
        gameWindowPositionWatcher_ = nullptr;
    }
}


void Application::getAllCommands(Array<CommandID>& commands)
{
    JUCEApplication::getAllCommands(commands);

    CommandID ids[] = {
        CommandIDs::EnableGameWindowPositionWatcher,
        CommandIDs::DisableGameWindowPositionWatcher,
    };

    commands.addArray(ids, numElementsInArray(ids));
}


void Application::getCommandInfo(CommandID commandID, ApplicationCommandInfo& result)
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


bool Application::perform(InvocationInfo const& info)
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

