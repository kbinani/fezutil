#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class FEZUtilApplication;

class TrayIcon : public SystemTrayIconComponent
{
public:
    TrayIcon(FEZUtilApplication* app, ApplicationCommandManager* commandManager);

    ~TrayIcon();

private:
    void mouseUp(MouseEvent const& e) override;

private:
    ApplicationCommandManager* const commandManager_;
    FEZUtilApplication* const app_;
};
