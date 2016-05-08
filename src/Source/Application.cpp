#include "./Application.hpp"
#include "./TrayIcon.hpp"
#include "./CommandIDs.hpp"
#include "./GameWindowPositionWatcher.hpp"
#include "./ReplaySaveFolderWatcher.hpp"
#include "./ConfigurationDialogComponent.hpp"
#include "./LookAndFeel.hpp"

Application::Application()
{
}


Application::~Application()
{
}


void Application::initialise(String const& commandLine)
{
    PropertiesFile::Options opt;

    opt.applicationName = ProjectInfo::projectName;
    opt.commonToAllUsers = false;
    opt.storageFormat = PropertiesFile::StorageFormat::storeAsXML;
    opt.filenameSuffix = ".xml";

    props_.setStorageParameters(opt);
    config_ = props_.getUserSettings();

    laf_ = new ::LookAndFeel();
    juce::LookAndFeel::setDefaultLookAndFeel(laf_);

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
    if (config_->getBoolValue(Configuration::kFixGameWindowPosition)) {
        bool const shouldReplace = gameWindowPositionWatcher_ != nullptr && !gameWindowPositionWatcher_->isWorking();

        if (gameWindowPositionWatcher_ == nullptr || shouldReplace) {
            gameWindowPositionWatcher_ = nullptr;
            gameWindowPositionWatcher_ = new GameWindowPositionWatcher();
        }
    } else {
        gameWindowPositionWatcher_ = nullptr;
    }

    if (config_->getBoolValue(Configuration::kBackupReplayFiles)) {
        File dest(config_->getValue(Configuration::kReplayFileBackupDestinaionFolder));
        bool const shouldReplace = replaySaveFolderWatcher_ != nullptr && (!replaySaveFolderWatcher_->isWorking() || replaySaveFolderWatcher_->getBackupDestinationFolder() != dest);

        if (replaySaveFolderWatcher_ == nullptr || shouldReplace) {
            replaySaveFolderWatcher_ = new ReplaySaveFolderWatcher(dest);
        }
    } else {
        replaySaveFolderWatcher_ = nullptr;
    }
}


void Application::getAllCommands(Array<CommandID>& commands)
{
    JUCEApplication::getAllCommands(commands);

    CommandID ids[] = {
        CommandIDs::EnableGameWindowPositionWatcher,
        CommandIDs::DisableGameWindowPositionWatcher,
        CommandIDs::EnableReplayFileBackup,
        CommandIDs::DisableReplayFileBackup,
    };

    commands.addArray(ids, numElementsInArray(ids));
}


void Application::getCommandInfo(CommandID commandID, ApplicationCommandInfo& result)
{
    switch (commandID) {
        case CommandIDs::DisableGameWindowPositionWatcher:
        {
            int flags = 0;
            result.setInfo(TRANS("Monitor Off"), TRANS("Disable fixing the game window"), String::empty, flags);
            break;
        }
        case CommandIDs::EnableGameWindowPositionWatcher:
        {
            int flags = 0;
            result.setInfo(TRANS("Monitor On"), TRANS("Enable fixing the game window"), String::empty, flags);
            break;
        }
        case CommandIDs::DisableReplayFileBackup:
        {
            int flags = 0;
            result.setInfo(TRANS("Backup Off"), TRANS("Disable backup replay files"), String::empty, flags);
        }
        case CommandIDs::EnableReplayFileBackup:
        {
            int flags = 0;
            result.setInfo(TRANS("Backup On"), TRANS("Enable backup replay files"), String::empty, flags);
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
            config_->setValue(Configuration::kFixGameWindowPosition, false);
            return true;
        }
        case CommandIDs::EnableGameWindowPositionWatcher:
        {
            config_->setValue(Configuration::kFixGameWindowPosition, true);
            return true;
        }
        case CommandIDs::DisableReplayFileBackup:
        {
            config_->setValue(Configuration::kBackupReplayFiles, false);
            return true;
        }
        case CommandIDs::EnableReplayFileBackup:
        {
            config_->setValue(Configuration::kBackupReplayFiles, true);
            return true;
        }
        default:
        {
            return JUCEApplication::perform(info);
        }
    }
}
