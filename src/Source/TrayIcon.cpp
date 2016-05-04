#include "./TrayIcon.hpp"

TrayIcon::TrayIcon(ApplicationCommandManager* commandManager)
    : commandManager_(commandManager)
{
    PNGImageFormat png;
    setIconImage(png.loadFrom(BinaryData::tray_icon_png, BinaryData::tray_icon_pngSize));
}


TrayIcon::~TrayIcon()
{}


void TrayIcon::mouseUp(MouseEvent const& e)
{
    PopupMenu menu;

    menu.addCommandItem(commandManager_, StandardApplicationCommandIDs::quit);

    PopupMenu::Options options;
    int const selectedItemId = menu.showMenu(options);

    if (selectedItemId == StandardApplicationCommandIDs::quit) {
        commandManager_->invoke(ApplicationCommandTarget::InvocationInfo(StandardApplicationCommandIDs::quit),
                                false); // asynchronously
    }
}
