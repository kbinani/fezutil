#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class ReplaySaveFolderWatcher
{
public:
    explicit ReplaySaveFolderWatcher(File const& backupDestinationFolder);

    ~ReplaySaveFolderWatcher();

    bool isWorking();

private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};
