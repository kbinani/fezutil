#include "./ReplaySaveFolderWatcher.hpp"
#include "./FEZGameInformation.hpp"
#include <thread>
#include <atomic>

class ReplaySaveFolderWatcher::Impl
{
public:
    explicit Impl(File const& backupDestinationFolder)
        : backupDestinationFolder_(backupDestinationFolder)
    {
        abortRequested_ = false;
        working_ = false;

        if (!backupDestinationFolder_.exists()) {
            return;
        }
        if (!backupDestinationFolder_.isDirectory()) {
            return;
        }

        File replayFolder = info_.lookupReplaySaveFolder();
        if (!replayFolder.exists()) {
            return;
        }

        worker_ = std::thread([this](File src, File dest) {
            working_ = true;

            while (!abortRequested_) {
                Array<File> files;
                src.findChildFiles(files,
                                   File::TypesOfFileToFind::findFiles,
                                   false, // searchRecursively
                                   "*.frd");
                for (File const& srcFile : files) {
                    File destFile = dest.getChildFile(srcFile.getFileName());
                    if (destFile.exists()) {
                        continue;
                    }
                    srcFile.copyFileTo(destFile);
                }

                std::this_thread::sleep_for(std::chrono::seconds(5));
            }

            working_ = false;
        }, replayFolder, backupDestinationFolder_);
    }

    ~Impl()
    {
        abortRequested_ = true;
        if (worker_.joinable()) {
            worker_.join();
        }
    }

    bool isWorking()
    {
        return working_;
    }

private:
    File const backupDestinationFolder_;
    std::thread worker_;
    std::atomic_bool abortRequested_;
    std::atomic_bool working_;
    FEZGameInformation info_;
};


ReplaySaveFolderWatcher::ReplaySaveFolderWatcher(File const& backupDestinationFolder)
    : impl_(std::make_unique<Impl>(backupDestinationFolder))
{
}


ReplaySaveFolderWatcher::~ReplaySaveFolderWatcher()
{
}


bool ReplaySaveFolderWatcher::isWorking()
{
    return impl_->isWorking();
}
