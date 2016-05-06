#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class Application;

class TrayIcon : public SystemTrayIconComponent
{
public:
    TrayIcon(Application* app, ApplicationCommandManager* commandManager);

    ~TrayIcon();

private:
    void mouseUp(MouseEvent const& e) override;

private:
    ApplicationCommandManager* const commandManager_;
    Application* const app_;
};
