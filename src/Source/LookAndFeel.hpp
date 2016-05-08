#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class LookAndFeel : public LookAndFeel_V3
{
public:
    void drawConcertinaPanelHeader(Graphics& g, Rectangle<int> const& area,
                                   bool isMouseOver, bool isMouseDown,
                                   ConcertinaPanel&, Component& panel) override;

    void drawButtonText(Graphics& g, TextButton& button,
                        bool isMouseOverButton, bool isButtonDown) override;

    void drawFileBrowserRow(Graphics& g, int width, int height,
                            String const& filename, Image* icon,
                            String const& fileSizeDescription,
                            String const& fileTimeDescription,
                            bool const isDirectory, bool const isItemSelected,
                            int const itemIndex, DirectoryContentsDisplayComponent& dcc) override;

    void drawPopupMenuItem(Graphics& g, Rectangle<int> const& area,
                           bool const isSeparator, bool const isActive,
                           bool const isHighlighted, bool const isTicked,
                           bool const hasSubMenu, String const& text,
                           String const& shortcutKeyText,
                           Drawable const* icon, Colour const* const textColourToUse) override;

    void drawLabel(Graphics& g, Label& label) override;

    void drawDocumentWindowTitleBar(DocumentWindow& window, Graphics& g,
                                    int w, int h, int titleSpaceX, int titleSpaceW,
                                    Image const* icon, bool drawTitleTextOnLeft) override;

    void drawPropertyPanelSectionHeader(Graphics& g, String const& name,
                                        bool isOpen, int width, int height) override;

    void drawPropertyComponentLabel(Graphics& g, int width, int height, PropertyComponent& component) override;

    void drawGroupComponentOutline(Graphics& g, int width, int height,
                                   String const& text, Justification const& position,
                                   GroupComponent& group) override;

    void drawTableHeaderColumn(Graphics& g, String const& columnName, int columnId,
                               int width, int height, bool isMouseOver, bool isMouseDown,
                               int columnFlags) override;

    void paintToolbarButtonLabel(Graphics& g, int x, int y, int width, int height,
                                 String const& text, ToolbarItemComponent& component) override;

    void drawProgressBar(Graphics& g, ProgressBar& progressBar,
                         int width, int height,
                         double progress, String const& textToShow) override;

    void drawKeymapChangeButton(Graphics& g, int width, int height,
                                Button& button, String const& keyDescription) override;

    void drawMenuBarItem(Graphics& g,
                         int width, int height,
                         int itemIndex,
                         String const& itemText,
                         bool isMouseOverItem, bool isMenuOpen, bool isMouseOverBar,
                         MenuBarComponent& menuBar) override;

    int getMenuBarItemWidth(MenuBarComponent& menuBar,
                            int itemIndex,
                            String const& itemText) override;

    void drawToggleButton(Graphics& g, ToggleButton& button,
                          bool isMouseOverButton, bool isButtonDown) override;
};
