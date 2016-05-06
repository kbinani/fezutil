#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class FEZGameInformation
{
public:
    Rectangle<int> lookupGameWindowBounds() const;

    void updateGameWindowBounds(Rectangle<int> const& bounds) const;

    File lookupReplaySaveFolder() const;
};
