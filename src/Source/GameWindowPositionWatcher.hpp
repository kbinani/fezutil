#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class GameWindowPositionWatcher
{
public:
    GameWindowPositionWatcher();

    ~GameWindowPositionWatcher();

    bool isWorking();

private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};
