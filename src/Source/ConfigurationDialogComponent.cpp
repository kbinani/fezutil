#include "./ConfigurationDialogComponent.hpp"
#include "./Configuration.hpp"
#include "./Application.hpp"

ConfigurationDialogComponent::ConfigurationDialogComponent(PropertiesFile* config)
    : config_(config)
{
    int const width = 640;
    int const height = 300;
    int const hMargin = 20;
    int const vMargin = 10;

    int const buttonWidth = 80;
    int const buttonHeight = 30;
    {
        buttonCancel_.setBounds(width - hMargin - buttonWidth,
                                height - 2 * vMargin - buttonHeight,
                                buttonWidth,
                                buttonHeight);
        buttonOk_.setBounds(width - hMargin * 2 - buttonWidth * 2,
                            height - 2 * vMargin - buttonHeight,
                            buttonWidth,
                            buttonHeight);

        buttonCancel_.setButtonText(TRANS("Cancel"));
        buttonOk_.setButtonText(TRANS("OK"));
        buttonCancel_.addShortcut(KeyPress(KeyPress::escapeKey));
        buttonOk_.addShortcut(KeyPress(KeyPress::returnKey));
        buttonCancel_.addListener(this);
        buttonOk_.addListener(this);
        addAndMakeVisible(buttonCancel_);
        addAndMakeVisible(buttonOk_);
    }

    auto* groupWindowPosition = new GroupComponent(String::empty, TRANS("Game window position"));
    {
        groupWindowPosition->setBounds(hMargin, vMargin, width - 2 * hMargin, 70);

        checkboxWatchGameWindowPosition_ = new ToggleButton();
        checkboxWatchGameWindowPosition_->setToggleState(config_->getBoolValue(Configuration::kFixGameWindowPosition), dontSendNotification);
        checkboxWatchGameWindowPosition_->setBounds(hMargin, 25, groupWindowPosition->getWidth() - 2 * hMargin, 20);
        checkboxWatchGameWindowPosition_->setButtonText(TRANS("Fix the game window position"));
        groupWindowPosition->addAndMakeVisible(checkboxWatchGameWindowPosition_);

        components_.add(groupWindowPosition);
        addAndMakeVisible(groupWindowPosition);
    }

    auto* groupReplayFilesBackup = new GroupComponent(String::empty, TRANS("Replay files backup"));
    {
        groupReplayFilesBackup->setBounds(hMargin,
                                          groupWindowPosition->getBottom() + vMargin,
                                          width - 2 * hMargin,
                                          buttonOk_.getY() - groupWindowPosition->getBottom() - 3 * vMargin);

        checkboxBackupReplayFiles_ = new ToggleButton();
        checkboxBackupReplayFiles_->setToggleState(config_->getBoolValue(Configuration::kBackupReplayFiles), dontSendNotification);
        checkboxBackupReplayFiles_->setBounds(hMargin, 25, groupReplayFilesBackup->getWidth() - 2 * hMargin, 20);
        checkboxBackupReplayFiles_->setButtonText(TRANS("Backup replay files"));
        groupReplayFilesBackup->addAndMakeVisible(checkboxBackupReplayFiles_);

        auto* labelReplayFileBackupDestinationFolder = new Label();
        labelReplayFileBackupDestinationFolder->setText(TRANS("Backup destination") + String(":"), dontSendNotification);
        labelReplayFileBackupDestinationFolder->setBounds(hMargin,
                                                          checkboxBackupReplayFiles_->getBottom() + vMargin,
                                                          groupReplayFilesBackup->getWidth() - 2 * hMargin,
                                                          20);
        components_.add(labelReplayFileBackupDestinationFolder);
        groupReplayFilesBackup->addAndMakeVisible(labelReplayFileBackupDestinationFolder);

        buttonReplayFileBackupDestinationFolder_ = new TextButton();
        buttonReplayFileBackupDestinationFolder_->setButtonText("...");
        buttonReplayFileBackupDestinationFolder_->setSize(30, 30);
        buttonReplayFileBackupDestinationFolder_->setBounds(groupReplayFilesBackup->getWidth() - hMargin - buttonReplayFileBackupDestinationFolder_->getWidth(),
                                                            labelReplayFileBackupDestinationFolder->getBottom() + vMargin,
                                                            buttonReplayFileBackupDestinationFolder_->getWidth(),
                                                            buttonReplayFileBackupDestinationFolder_->getHeight());
        buttonReplayFileBackupDestinationFolder_->addListener(this);
        groupReplayFilesBackup->addAndMakeVisible(buttonReplayFileBackupDestinationFolder_);

        textboxReplayFileBackupDestinationFolder_ = new Label();
        textboxReplayFileBackupDestinationFolder_->setText(config_->getValue(Configuration::kReplayFileBackupDestinaionFolder), dontSendNotification);
        textboxReplayFileBackupDestinationFolder_->setBounds(hMargin,
                                                             labelReplayFileBackupDestinationFolder->getBottom() + vMargin,
                                                             groupReplayFilesBackup->getWidth() - 3 * hMargin - buttonReplayFileBackupDestinationFolder_->getWidth(),
                                                             30);
        textboxReplayFileBackupDestinationFolder_->setEditable(true);
        groupReplayFilesBackup->addAndMakeVisible(textboxReplayFileBackupDestinationFolder_);

        components_.add(groupReplayFilesBackup);
        addAndMakeVisible(groupReplayFilesBackup);
    }

    setSize(width, height);
}


void ConfigurationDialogComponent::buttonClicked(Button* button)
{
    DialogWindow* dialog = findParentComponentOfClass<DialogWindow>();

    if (button == &buttonCancel_) {
        if (dialog) {
            dialog->exitModalState(0);
        }
    } else if (button == &buttonOk_) {
        if (dialog) {
            config_->setValue(Configuration::kReplayFileBackupDestinaionFolder, textboxReplayFileBackupDestinationFolder_->getText());
            config_->setValue(Configuration::kBackupReplayFiles, checkboxBackupReplayFiles_->getToggleState());

            config_->setValue(Configuration::kFixGameWindowPosition, checkboxWatchGameWindowPosition_->getToggleState());

            dialog->exitModalState(1);
        }
    } else if (button == buttonReplayFileBackupDestinationFolder_) {
        FileChooser fc(TRANS("Select backup destination folder"));
        if (fc.browseForDirectory()) {
            textboxReplayFileBackupDestinationFolder_->setText(fc.getResult().getFullPathName(), dontSendNotification);
        }
    }
}
