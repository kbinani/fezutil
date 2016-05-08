#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class Configuration
{
    Configuration() = delete;

public:
    static String const kReplayFileBackupDestinaionFolder;
    static String const kBackupReplayFiles;
    static String const kFixGameWindowPosition;
};
