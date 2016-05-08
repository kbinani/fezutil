#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class ReplaySaveFolderWatcher
{
public:
    explicit ReplaySaveFolderWatcher(File const& backupDestinationFolder);

    ~ReplaySaveFolderWatcher();

    bool isWorking();

    File const& getBackupDestinationFolder() const;

private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};
