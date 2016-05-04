#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class TrayIcon : public SystemTrayIconComponent
{
public:
    explicit TrayIcon(ApplicationCommandManager* commandManager);

    ~TrayIcon();

private:
    void mouseUp(MouseEvent const& e) override;

private:
    ApplicationCommandManager* const commandManager_;
};
