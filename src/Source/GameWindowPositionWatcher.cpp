#include "./GameWindowPositionWatcher.hpp"
#include "./FEZGameInformation.hpp"
#include <thread>
#include <atomic>

class GameWindowPositionWatcher::Impl
{
public:
    Impl()
    {
        abortRequested_ = false;
        isWorking_ = true;

        Rectangle<int> initialBounds = info_.lookupGameWindowBounds();

        if (!initialBounds.isEmpty()) {
            isWorking_ = false;
            return;
        }

        thread_ = std::thread([this](Rectangle<int> initialBounds) {
            while (!this->abortRequested_) {
                std::this_thread::sleep_for(std::chrono::milliseconds(1));

                auto const bounds = info_.lookupGameWindowBounds();

                if (bounds.isEmpty()) {
                    break;
                }

                if (bounds != initialBounds) {
                    info_.updateGameWindowBounds(initialBounds);
                }
            }

            isWorking_ = false;
        }, initialBounds);
    }

    ~Impl()
    {
        abortRequested_ = true;
        if (thread_.joinable()) {
            thread_.join();
        }
    }

    bool isWorking()
    {
        return isWorking_.load();
    }

private:
    std::thread thread_;
    std::atomic_bool abortRequested_;
    std::atomic_bool isWorking_;
    FEZGameInformation info_;
};


GameWindowPositionWatcher::GameWindowPositionWatcher()
    : impl_(std::make_unique<Impl>())
{}


GameWindowPositionWatcher::~GameWindowPositionWatcher()
{}


bool GameWindowPositionWatcher::isWorking()
{
    return impl_->isWorking();
}
