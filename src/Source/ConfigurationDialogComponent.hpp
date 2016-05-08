#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "./Configuration.hpp"

class Application;

class ConfigurationDialogComponent
    : public Component
    , public Button::Listener
{
public:
    explicit ConfigurationDialogComponent(PropertiesFile* config);

private:
    void buttonClicked(Button* button) override;

public:
    OwnedArray<Component> components_;
    TextButton buttonOk_;
    TextButton buttonCancel_;
    ScopedPointer<TextButton> buttonReplayFileBackupDestinationFolder_;
    ScopedPointer<Label> textboxReplayFileBackupDestinationFolder_;
    ScopedPointer<ToggleButton> checkboxWatchGameWindowPosition_;
    ScopedPointer<ToggleButton> checkboxBackupReplayFiles_;
    PropertiesFile* config_;
};
