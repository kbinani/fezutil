#include "./TrayIcon.hpp"
#include "./FEZUtilApplication.hpp"
#include "./CommandIDs.hpp"

TrayIcon::TrayIcon(FEZUtilApplication* app, ApplicationCommandManager* commandManager)
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
    };

    menu.addSectionHeader(ProjectInfo::projectName);
    menu.addSeparator();

    bool const ticked = app_->isMonitoringGameWindow();
    menu.addItem(ToggleGameWindowPositionWatcher,
                 TRANS("Monitor Game Window"),
                 true, // isEnabled
                 ticked); // isTicked

    menu.addSeparator();
    menu.addCommandItem(commandManager_, StandardApplicationCommandIDs::quit);

    PopupMenu::Options options;
    int const selectedItemId = menu.showMenu(options);

    if (selectedItemId == StandardApplicationCommandIDs::quit) {
        commandManager_->invoke(StandardApplicationCommandIDs::quit, false);
    } else if (selectedItemId == ToggleGameWindowPositionWatcher) {
        CommandID command = ticked ? CommandIDs::DisableGameWindowPositionWatcher : CommandIDs::EnableGameWindowPositionWatcher;
        commandManager_->invoke(command, false);
    }
}
