#include "./GameWindowPositionWatcher.hpp"
#include <thread>
#include <atomic>
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

class GameWindowPositionWatcher::Impl
{
public:
    Impl()
    {
        abortRequested_ = false;
        isWorking_ = true;

        HWND fezWindow = FindWindowW(L"MainWindow", L"Fantasy Earth Zero");
        if (!fezWindow) {
            isWorking_ = false;
            return;
        }

        thread_ = std::thread([this](HWND fezWindow) {
            auto const getWindowBounds = [](HWND handle) {
                RECT bounds;
                bounds.top = 0;
                bounds.left = 0;
                bounds.bottom = 0;
                bounds.right = 0;
                if (!GetWindowRect(handle, &bounds)) {
                    return bounds;
                }
                return bounds;
            };

            RECT const initialBounds = getWindowBounds(fezWindow);

            if (IsRectEmpty(&initialBounds)) {
                isWorking_ = false;
                return;
            }

            while (!this->abortRequested_) {
                std::this_thread::sleep_for(std::chrono::milliseconds(1));

                RECT const bounds = getWindowBounds(fezWindow);

                if (IsRectEmpty(&bounds)) {
                    break;
                }

                if (bounds.top != initialBounds.top || bounds.left != initialBounds.left) {
                    MoveWindow(fezWindow,
                               initialBounds.left, initialBounds.top,
                               bounds.right - bounds.left, bounds.bottom - bounds.top,
                               FALSE); // bRepaint
                }
            }

            isWorking_ = false;
        }, fezWindow);
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
