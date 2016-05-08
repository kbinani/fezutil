#include "./LookAndFeel.hpp"

namespace
{
    //! Draw string with juce::TextLayout.
    /*!
     * juce::TextLayout can draw correctly non-ASCII string (juce::Graphics::drawText can't).
     */
    void doDrawText(Graphics& g, String text, int x, int y, int width, int height, Justification justification, Colour colour)
    {
        Graphics::ScopedSaveState state(g);

        AffineTransform t;
        t = t.translated(x, y);
        g.addTransform(t);

        AttributedString s;
        s.setJustification(justification);
        s.append(text, g.getCurrentFont(), colour);

        TextLayout layout;
        layout.createLayout(s, width, height);

        layout.draw(g, Rectangle<float>(width, height));
    }


    bool isASCIIOnly(String const& text)
    {
        char const* raw = text.toRawUTF8();
        return CharPointer_ASCII::isValidString(raw, strlen(raw));
    }


    void drawText(Graphics& g, String text, int x, int y, int width, int height, Justification justification, Colour colour, bool useEllipsesIfTooBig = false)
    {
        Graphics::ScopedSaveState state(g);

        if (isASCIIOnly(text)) {
            g.setColour(colour);
            g.drawText(text, x, y, width, height, justification, useEllipsesIfTooBig);
        } else {
            doDrawText(g, text, x, y, width, height, justification, colour);
        }
    }


    void drawText(Graphics& g, String text, Rectangle<int> const& bounds, Justification justification, Colour colour, bool useEllipsesIfTooBig = false)
    {
        drawText(g, text, bounds.getX(), bounds.getY(), bounds.getWidth(), bounds.getHeight(), justification, colour, useEllipsesIfTooBig);
    }


    void drawFittedText(
        Graphics& g,
        String const& text,
        int x, int y, int width, int height, Justification justificationFlags,
        Colour colour, int maximumNumberOfLines, float const minimumHorizontalScale = 0.0f)
    {
        Graphics::ScopedSaveState state(g);

        if (isASCIIOnly(text)) {
            g.setColour(colour);
            g.drawFittedText(text, x, y, width, height, justificationFlags, maximumNumberOfLines, minimumHorizontalScale);
        } else {
            doDrawText(g, text, x, y, width, height, justificationFlags, colour);
        }
    }


    void drawFittedText(
        Graphics& g,
        String const& text,
        Rectangle<int> const& bounds, Justification justificationFlags,
        Colour colour, int maximumNumberOfLines, float const minimumHorizontalScale = 0.0f)
    {
        drawFittedText(g, text, bounds.getX(), bounds.getY(), bounds.getWidth(), bounds.getHeight(), justificationFlags, colour, maximumNumberOfLines, minimumHorizontalScale);
    }

} // namespace

void ::LookAndFeel::drawConcertinaPanelHeader(
    Graphics& g, Rectangle<int> const& area,
    bool isMouseOver, bool isMouseDown,
    ConcertinaPanel&, Component& panel)
{
    const Colour bkg(Colours::grey);

    g.setGradientFill(ColourGradient(
        Colours::white.withAlpha(isMouseOver ? 0.4f : 0.2f), 0, (float)area.getY(),
        Colours::darkgrey.withAlpha(0.1f), 0, (float)area.getBottom(), false));
    g.fillAll();

    g.setColour(bkg.contrasting().withAlpha(0.1f));
    g.fillRect(area.withHeight(1));
    g.fillRect(area.withTop(area.getBottom() - 1));

    g.setFont(Font(area.getHeight() * 0.6f).boldened());
    drawText(g, panel.getName(), Rectangle<int>(4, 0, area.getWidth() - 6, area.getHeight()), Justification::centredLeft, bkg.contrasting());
}


void ::LookAndFeel::drawButtonText(
    Graphics& g, TextButton& button,
    bool isMouseOverButton, bool isButtonDown)
{
    Font font(getTextButtonFont(button, button.getHeight()));
    g.setFont(font);

    Colour textColour = button.findColour(button.getToggleState() ? TextButton::textColourOnId
                                                                  : TextButton::textColourOffId)
                            .withMultipliedAlpha(button.isEnabled() ? 1.0f : 0.5f);

    const int yIndent = jmin(4, button.proportionOfHeight(0.3f));
    const int cornerSize = jmin(button.getHeight(), button.getWidth()) / 2;

    const int fontHeight = roundToInt(font.getHeight() * 0.6f);
    const int leftIndent = jmin(fontHeight, 2 + cornerSize / (button.isConnectedOnLeft() ? 4 : 2));
    const int rightIndent = jmin(fontHeight, 2 + cornerSize / (button.isConnectedOnRight() ? 4 : 2));

    drawFittedText(g, button.getButtonText(), leftIndent, yIndent,
        button.getWidth() - leftIndent - rightIndent,
        button.getHeight() - yIndent * 2,
        Justification::centred, textColour, 2);
}


void ::LookAndFeel::drawFileBrowserRow(
    Graphics& g, int width, int height,
    String const& filename, Image* icon,
    String const& fileSizeDescription,
    String const& fileTimeDescription,
    bool const isDirectory, bool const isItemSelected,
    int const itemIndex, DirectoryContentsDisplayComponent& dcc)
{
    Component* const fileListComp = dynamic_cast<Component*>(&dcc);

    if (isItemSelected) {
        g.fillAll(fileListComp != nullptr ? fileListComp->findColour(DirectoryContentsDisplayComponent::highlightColourId)
                                          : findColour(DirectoryContentsDisplayComponent::highlightColourId));
    }

    int const x = 32;
    g.setColour(Colours::black);

    if (icon != nullptr && icon->isValid()) {
        g.drawImageWithin(
            *icon, 2, 2, x - 4, height - 4,
            RectanglePlacement::centred | RectanglePlacement::onlyReduceInSize,
            false);
    } else {
        if (Drawable const* d = isDirectory ? getDefaultFolderImage()
                                            : getDefaultDocumentFileImage()) {
            d->drawWithin(
                g, Rectangle<float>(2.0f, 2.0f, x - 4.0f, height - 4.0f),
                RectanglePlacement::centred | RectanglePlacement::onlyReduceInSize, 1.0f);
        }
    }

    Colour textColour = fileListComp != nullptr ? fileListComp->findColour(DirectoryContentsDisplayComponent::textColourId)
                                                : findColour(DirectoryContentsDisplayComponent::textColourId);
    g.setFont(height * 0.7f);

    if (width > 450 && !isDirectory) {
        int const sizeX = roundToInt(width * 0.7f);
        int const dateX = roundToInt(width * 0.8f);

        drawText(g, filename, x, 0, sizeX - x, height, Justification::centredLeft, textColour);

        g.setFont(height * 0.5f);
        g.setColour(Colours::darkgrey);

        if (!isDirectory) {
            drawText(
                g,
                fileSizeDescription,
                sizeX, 0, dateX - sizeX - 8, height,
                Justification::centredRight, Colours::darkgrey);
            drawText(
                g,
                fileTimeDescription,
                dateX, 0, width - 8 - dateX, height,
                Justification::centredRight, Colours::darkgrey);
        }
    } else {
        drawText(
            g,
            filename,
            x, 0, width - x, height,
            Justification::centredLeft, textColour);
    }
}


void ::LookAndFeel::drawPopupMenuItem(
    Graphics& g, Rectangle<int> const& area,
    bool const isSeparator, bool const isActive,
    bool const isHighlighted, bool const isTicked,
    bool const hasSubMenu, String const& text,
    String const& shortcutKeyText,
    Drawable const* icon, Colour const* const textColourToUse)
{
    Colour currentColour;
    auto setColour = [&currentColour, &g](Colour c) {
        g.setColour(c);
        currentColour = c;
    };

    if (isSeparator) {
        Rectangle<int> r(area.reduced(5, 0));
        r.removeFromTop(r.getHeight() / 2 - 1);

        setColour(Colour(0x33000000));
        g.fillRect(r.removeFromTop(1));

        setColour(Colour(0x66ffffff));
        g.fillRect(r.removeFromTop(1));
    } else {
        Colour textColour(findColour(PopupMenu::textColourId));

        if (textColourToUse != nullptr) {
            textColour = *textColourToUse;
        }

        Rectangle<int> r(area.reduced(1));

        if (isHighlighted) {
            setColour(findColour(PopupMenu::highlightedBackgroundColourId));
            g.fillRect(r);

            setColour(findColour(PopupMenu::highlightedTextColourId));
        } else {
            setColour(textColour);
        }

        if (!isActive) {
            g.setOpacity(0.3f);
        }

        Font font(getPopupMenuFont());

        const float maxFontHeight = area.getHeight() / 1.3f;

        if (font.getHeight() > maxFontHeight) {
            font.setHeight(maxFontHeight);
        }

        g.setFont(font);

        Rectangle<float> iconArea(r.removeFromLeft((r.getHeight() * 5) / 4).reduced(3).toFloat());

        if (icon != nullptr) {
            icon->drawWithin(g, iconArea, RectanglePlacement::centred | RectanglePlacement::onlyReduceInSize, 1.0f);
        } else if (isTicked) {
            const Path tick(getTickShape(1.0f));
            g.fillPath(tick, tick.getTransformToScaleToFit(iconArea, true));
        }

        if (hasSubMenu) {
            const float arrowH = 0.6f * getPopupMenuFont().getAscent();

            const float x = (float)r.removeFromRight((int)arrowH).getX();
            const float halfH = (float)r.getCentreY();

            Path p;
            p.addTriangle(x, halfH - arrowH * 0.5f,
                x, halfH + arrowH * 0.5f,
                x + arrowH * 0.6f, halfH);

            g.fillPath(p);
        }

        r.removeFromRight(3);

        drawText(g, text, r, Justification::centredLeft, currentColour, true);

        if (shortcutKeyText.isNotEmpty()) {
            Font f2(font);
            f2.setHeight(f2.getHeight() * 0.75f);
            f2.setHorizontalScale(0.95f);
            g.setFont(f2);

            drawText(g, shortcutKeyText, r, Justification::centredRight, currentColour, true);
        }
    }
}


void ::LookAndFeel::drawLabel(Graphics& g, Label& label)
{
    g.fillAll(label.findColour(Label::backgroundColourId));

    if (!label.isBeingEdited()) {
        const float alpha = label.isEnabled() ? 1.0f : 0.5f;
        const Font font(getLabelFont(label));

        g.setFont(font);

        Rectangle<int> textArea(label.getBorderSize().subtractedFrom(label.getLocalBounds()));

        drawFittedText(
            g,
            label.getText(),
            textArea, label.getJustificationType(),
            label.findColour(Label::textColourId).withMultipliedAlpha(alpha),
            jmax(1, (int)(textArea.getHeight() / font.getHeight())),
            label.getMinimumHorizontalScale());

        g.setColour(label.findColour(Label::outlineColourId).withMultipliedAlpha(alpha));
    } else if (label.isEnabled()) {
        g.setColour(label.findColour(Label::outlineColourId));
    }

    g.drawRect(label.getLocalBounds());
}


void ::LookAndFeel::drawDocumentWindowTitleBar(
    DocumentWindow& window, Graphics& g,
    int w, int h, int titleSpaceX, int titleSpaceW,
    Image const* icon, bool drawTitleTextOnLeft)
{
    const bool isActive = window.isActiveWindow();

    g.setGradientFill(ColourGradient(window.getBackgroundColour(),
        0.0f, 0.0f,
        window.getBackgroundColour().contrasting(isActive ? 0.15f : 0.05f),
        0.0f, (float)h, false));
    g.fillAll();

    Font font(h * 0.65f, Font::bold);
    g.setFont(font);

    int textW = font.getStringWidth(window.getName());
    int iconW = 0;
    int iconH = 0;

    if (icon != nullptr) {
        iconH = (int)font.getHeight();
        iconW = icon->getWidth() * iconH / icon->getHeight() + 4;
    }

    textW = jmin(titleSpaceW, textW + iconW);
    int textX = drawTitleTextOnLeft
        ? titleSpaceX
        : jmax(titleSpaceX, (w - textW) / 2);

    if (textX + textW > titleSpaceX + titleSpaceW) {
        textX = titleSpaceX + titleSpaceW - textW;
    }

    if (icon != nullptr) {
        g.setOpacity(isActive ? 1.0f : 0.6f);
        g.drawImageWithin(*icon, textX, (h - iconH) / 2, iconW, iconH,
            RectanglePlacement::centred, false);
        textX += iconW;
        textW -= iconW;
    }

    Colour colour;
    if (window.isColourSpecified(DocumentWindow::textColourId) || isColourSpecified(DocumentWindow::textColourId)) {
        colour = window.findColour(DocumentWindow::textColourId);
    } else {
        colour = window.getBackgroundColour().contrasting(isActive ? 0.7f : 0.4f);
    }

    drawText(g, window.getName(), textX, 0, textW, h, Justification::centredLeft, colour, true);
}


void ::LookAndFeel::drawPropertyPanelSectionHeader(
    Graphics& g, String const& name,
    bool isOpen, int width, int height)
{
    const float buttonSize = height * 0.75f;
    const float buttonIndent = (height - buttonSize) * 0.5f;

    drawTreeviewPlusMinusBox(g, Rectangle<float>(buttonIndent, buttonIndent, buttonSize, buttonSize), Colours::white, isOpen, false);

    const int textX = (int)(buttonIndent * 2.0f + buttonSize + 2.0f);

    g.setFont(Font(height * 0.7f, Font::bold));
    drawText(g, name, textX, 0, width - textX - 4, height, Justification::centredLeft, Colours::black, true);
}


void:: LookAndFeel::drawPropertyComponentLabel(
    Graphics& g,
    int width, int height,
    PropertyComponent& component)
{
    Colour colour = component.findColour(PropertyComponent::labelTextColourId)
                        .withMultipliedAlpha(component.isEnabled() ? 1.0f : 0.6f);

    g.setFont(jmin(height, 24) * 0.65f);

    const Rectangle<int> r(getPropertyComponentContentPosition(component));

    drawFittedText(
        g, component.getName(),
        3, r.getY(), r.getX() - 5, r.getHeight(),
        Justification::centredLeft, colour, 2);
}


void ::LookAndFeel::drawGroupComponentOutline(
    Graphics& g, int width, int height,
    String const& text, Justification const& position,
    GroupComponent& group)
{
    const float textH = 15.0f;
    const float indent = 3.0f;
    const float textEdgeGap = 4.0f;
    float cs = 5.0f;

    Font f(textH);

    Path p;
    float x = indent;
    float y = f.getAscent() - 3.0f;
    float w = jmax(0.0f, width - x * 2.0f);
    float h = jmax(0.0f, height - y - indent);
    cs = jmin(cs, w * 0.5f, h * 0.5f);
    const float cs2 = 2.0f * cs;

    float textW = text.isEmpty() ? 0 : jlimit(0.0f, jmax(0.0f, w - cs2 - textEdgeGap * 2), f.getStringWidth(text) + textEdgeGap * 2.0f);
    float textX = cs + textEdgeGap;

    if (position.testFlags(Justification::horizontallyCentred)) {
        textX = cs + (w - cs2 - textW) * 0.5f;
    } else if (position.testFlags(Justification::right)) {
        textX = w - cs - textW - textEdgeGap;
    }

    p.startNewSubPath(x + textX + textW, y);
    p.lineTo(x + w - cs, y);

    p.addArc(x + w - cs2, y, cs2, cs2, 0, float_Pi * 0.5f);
    p.lineTo(x + w, y + h - cs);

    p.addArc(x + w - cs2, y + h - cs2, cs2, cs2, float_Pi * 0.5f, float_Pi);
    p.lineTo(x + cs, y + h);

    p.addArc(x, y + h - cs2, cs2, cs2, float_Pi, float_Pi * 1.5f);
    p.lineTo(x, y + cs);

    p.addArc(x, y, cs2, cs2, float_Pi * 1.5f, float_Pi * 2.0f);
    p.lineTo(x + textX, y);

    const float alpha = group.isEnabled() ? 1.0f : 0.5f;

    g.setColour(group.findColour(GroupComponent::outlineColourId)
                    .withMultipliedAlpha(alpha));

    g.strokePath(p, PathStrokeType(2.0f));

    Colour colour = group.findColour(GroupComponent::textColourId)
                        .withMultipliedAlpha(alpha);
    g.setFont(f);
    drawText(g, text,
        roundToInt(x + textX), 0,
        roundToInt(textW),
        roundToInt(textH),
        Justification::centred, colour, true);
}


void ::LookAndFeel::drawTableHeaderColumn(
    Graphics& g, String const& columnName, int columnId,
    int width, int height, bool isMouseOver, bool isMouseDown,
    int columnFlags)
{
    if (isMouseDown) {
        g.fillAll(Colour(0x8899aadd));
    } else if (isMouseOver) {
        g.fillAll(Colour(0x5599aadd));
    }

    Rectangle<int> area(width, height);
    area.reduce(4, 0);

    if ((columnFlags & (TableHeaderComponent::sortedForwards | TableHeaderComponent::sortedBackwards)) != 0) {
        Path sortArrow;
        sortArrow.addTriangle(0.0f, 0.0f,
            0.5f, (columnFlags & TableHeaderComponent::sortedForwards) != 0 ? -0.8f : 0.8f,
            1.0f, 0.0f);

        g.setColour(Colour(0x99000000));
        g.fillPath(sortArrow, sortArrow.getTransformToScaleToFit(area.removeFromRight(height / 2).reduced(2).toFloat(), true));
    }

    g.setFont(Font(height * 0.5f, Font::bold));
    drawFittedText(g, columnName, area, Justification::centredLeft, Colours::black, 1);
}


void ::LookAndFeel::paintToolbarButtonLabel(
    Graphics& g, int x, int y, int width, int height,
    String const& text, ToolbarItemComponent& component)
{
    Colour colour = component.findColour(Toolbar::labelTextColourId, true)
                        .withAlpha(component.isEnabled() ? 1.0f : 0.25f);

    const float fontHeight = jmin(14.0f, height * 0.85f);
    g.setFont(fontHeight);

    drawFittedText(g, text,
        x, y, width, height,
        Justification::centred,
        colour,
        jmax(1, height / (int)fontHeight));
}


void ::LookAndFeel::drawProgressBar(
    Graphics& g, ProgressBar& progressBar,
    int width, int height,
    double progress, String const& textToShow)
{
    const Colour background(progressBar.findColour(ProgressBar::backgroundColourId));
    const Colour foreground(progressBar.findColour(ProgressBar::foregroundColourId));

    g.fillAll(background);

    if (progress >= 0.0f && progress < 1.0f) {
        drawGlassLozenge(g, 1.0f, 1.0f,
            (float)jlimit(0.0, width - 2.0, progress * (width - 2.0)),
            (float)(height - 2),
            foreground,
            0.5f, 0.0f,
            true, true, true, true);
    } else {
        // spinning bar..
        g.setColour(foreground);

        const int stripeWidth = height * 2;
        const int position = (int)(Time::getMillisecondCounter() / 15) % stripeWidth;

        Path p;

        for (float x = (float)(-position); x < width + stripeWidth; x += stripeWidth) {
            p.addQuadrilateral(x, 0.0f,
                x + stripeWidth * 0.5f, 0.0f,
                x, (float)height,
                x - stripeWidth * 0.5f, (float)height);
        }

        Image im(Image::ARGB, width, height, true);

        {
            Graphics g2(im);
            drawGlassLozenge(g2, 1.0f, 1.0f,
                (float)(width - 2),
                (float)(height - 2),
                foreground,
                0.5f, 0.0f,
                true, true, true, true);
        }

        g.setTiledImageFill(im, 0, 0, 0.85f);
        g.fillPath(p);
    }

    if (textToShow.isNotEmpty()) {
        Colour colour = Colour::contrasting(background, foreground);
        g.setFont(height * 0.6f);

        drawText(g, textToShow, 0, 0, width, height, Justification::centred, colour, false);
    }
}


void ::LookAndFeel::drawKeymapChangeButton(
    Graphics& g, int width, int height,
    Button& button, String const& keyDescription)
{
    const Colour textColour(button.findColour(0x100ad01 /*KeyMappingEditorComponent::textColourId*/, true));

    if (keyDescription.isNotEmpty()) {
        if (button.isEnabled()) {
            g.setColour(textColour.withAlpha(button.isDown() ? 0.4f : (button.isOver() ? 0.2f : 0.1f)));
            g.fillRoundedRectangle(button.getLocalBounds().toFloat(), 4.0f);
            g.drawRoundedRectangle(button.getLocalBounds().toFloat(), 4.0f, 1.0f);
        }

        g.setFont(height * 0.6f);
        drawFittedText(g, keyDescription, 4, 0, width - 8, height, Justification::centred, textColour, 1);
    } else {
        const float thickness = 7.0f;
        const float indent = 22.0f;

        Path p;
        p.addEllipse(0.0f, 0.0f, 100.0f, 100.0f);
        p.addRectangle(indent, 50.0f - thickness, 100.0f - indent * 2.0f, thickness * 2.0f);
        p.addRectangle(50.0f - thickness, indent, thickness * 2.0f, 50.0f - indent - thickness);
        p.addRectangle(50.0f - thickness, 50.0f + thickness, thickness * 2.0f, 50.0f - indent - thickness);
        p.setUsingNonZeroWinding(false);

        g.setColour(textColour.darker(0.1f).withAlpha(button.isDown() ? 0.7f : (button.isOver() ? 0.5f : 0.3f)));
        g.fillPath(p, p.getTransformToScaleToFit(2.0f, 2.0f, width - 4.0f, height - 4.0f, true));
    }

    if (button.hasKeyboardFocus(false)) {
        g.setColour(textColour.withAlpha(0.4f));
        g.drawRect(0, 0, width, height);
    }
}


void ::LookAndFeel::drawMenuBarItem(
    Graphics& g,
    int width, int height,
    int itemIndex,
    String const& itemText,
    bool isMouseOverItem, bool isMenuOpen, bool isMouseOverBar,
    MenuBarComponent& menuBar)
{
    Colour textColour;
    if (!menuBar.isEnabled()) {
        textColour = menuBar.findColour(PopupMenu::textColourId).withMultipliedAlpha(0.5f);
    } else if (isMenuOpen || isMouseOverItem) {
        g.fillAll(menuBar.findColour(PopupMenu::highlightedBackgroundColourId));
        textColour = menuBar.findColour(PopupMenu::highlightedTextColourId);
    } else {
        textColour = menuBar.findColour(PopupMenu::textColourId);
    }

    g.setFont(getMenuBarFont(menuBar, itemIndex, itemText));
    drawFittedText(g, itemText, 0, 0, width, height, Justification::centred, textColour, 1);
}


int ::LookAndFeel::getMenuBarItemWidth(
    MenuBarComponent& menuBar,
    int itemIndex,
    String const& itemText)
{
    return getMenuBarFont(menuBar, itemIndex, itemText).getStringWidth(itemText) + menuBar.getHeight();
}


void ::LookAndFeel::drawToggleButton(Graphics& g, ToggleButton& button,
                                     bool isMouseOverButton, bool isButtonDown)
{
    if (button.hasKeyboardFocus(true)) {
        g.setColour(button.findColour(TextEditor::focusedOutlineColourId));
        g.drawRect(0, 0, button.getWidth(), button.getHeight());
    }

    float const fontSize = jmin(15.0f, button.getHeight() * 0.75f);
    float const tickWidth = fontSize * 1.1f;

    drawTickBox(g, button, 4.0f, (button.getHeight() - tickWidth) * 0.5f,
                tickWidth, tickWidth,
                button.getToggleState(),
                button.isEnabled(),
                isMouseOverButton,
                isButtonDown);

    g.setFont(fontSize);

    if (!button.isEnabled()) {
        g.setOpacity(0.5f);
    }

    int const textX = (int)tickWidth + 5;

    drawFittedText(g,
                   button.getButtonText(),
                   textX, 0,
                   button.getWidth() - textX - 2, button.getHeight(),
                   Justification::centredLeft,
                   button.findColour(ToggleButton::textColourId),
                   10); // maximumNumberOfLines
}
