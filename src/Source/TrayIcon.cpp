#include "./TrayIcon.hpp"
#include "./Application.hpp"
#include "./CommandIDs.hpp"
#include "./ConfigurationDialogComponent.hpp"

TrayIcon::TrayIcon(Application* app, ApplicationCommandManager* commandManager)
    : commandManager_(commandManager)
    , app_(app)
{
    PNGImageFormat png;
    setIconImage(png.loadFrom(BinaryData::tray_icon_png, BinaryData::tray_icon_pngSize));
}


TrayIcon::~TrayIcon()
{}


void TrayIcon::mouseUp(MouseEvent const& e)
{
    PopupMenu menu;

    enum Command
    {
        ToggleGameWindowPositionWatcher = 1,
        ToggleBackupReplayFiles,
        Settings,
    };

    PropertiesFile const* config = app_->getConfiguration();

    menu.addSectionHeader(ProjectInfo::projectName);
    menu.addSeparator();

    menu.addItem(ToggleGameWindowPositionWatcher,
                 TRANS(L"�Q�[���E�B���h�E�̈ʒu���Œ�"),
                 true, // isEnabled
                 config->getBoolValue(Configuration::kFixGameWindowPosition)); // isTicked
    menu.addItem(ToggleBackupReplayFiles,
                 TRANS(L"��ꃊ�v���C�t�@�C�����o�b�N�A�b�v"),
                 true, // isEnabled
                 config->getBoolValue(Configuration::kBackupReplayFiles));

    menu.addSeparator();

    menu.addItem(Settings,
                 TRANS(L"�ݒ�"),
                 true, // isEnabled
                 false); // isTicked

    menu.addSeparator();
    menu.addCommandItem(commandManager_, StandardApplicationCommandIDs::quit, TRANS(L"�I��"));

    PopupMenu::Options options;
    int const selectedItemId = menu.showMenu(options);

    if (selectedItemId == StandardApplicationCommandIDs::quit) {
        commandManager_->invoke(StandardApplicationCommandIDs::quit, false);
    } else if (selectedItemId == ToggleGameWindowPositionWatcher) {
        CommandID command = config->getBoolValue(Configuration::kFixGameWindowPosition) ? CommandIDs::DisableGameWindowPositionWatcher : CommandIDs::EnableGameWindowPositionWatcher;
        commandManager_->invoke(command, false);
    } else if (selectedItemId == ToggleBackupReplayFiles) {
        CommandID command = config->getBoolValue(Configuration::kBackupReplayFiles) ? CommandIDs::DisableReplayFileBackup : CommandIDs::EnableReplayFileBackup;
        commandManager_->invoke(command, false);
    } else if (selectedItemId == Settings) {
        ScopedPointer<ConfigurationDialogComponent> comp = new ConfigurationDialogComponent(app_->getMutableConfiguration());

        DialogWindow::LaunchOptions opt;
        opt.content.setNonOwned(comp);
        opt.resizable = false;
        opt.dialogTitle = TRANS(L"�ݒ�");
        opt.runModal();
    }
}
