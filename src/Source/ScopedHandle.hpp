#pragma once
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <algorithm> // std::swap

class ScopedHandle
{
public:
    ScopedHandle(HANDLE h)
        : handle_(h)
    {
    }

    ScopedHandle(ScopedHandle const&) = delete;
    ScopedHandle& operator = (ScopedHandle const&) = delete;

    ScopedHandle(ScopedHandle&& lhs)
    {
        handle_ = lhs.handle_;
        lhs.handle_ = NULL;
    }

    ScopedHandle& operator = (ScopedHandle&& lhs)
    {
        std::swap(handle_, lhs.handle_);
        return *this;
    }

    ~ScopedHandle()
    {
        if (handle_) {
            CloseHandle(handle_);
            handle_ = NULL;
        }
    }

    operator HANDLE () const
    {
        return handle_;
    }

private:
    HANDLE handle_;
};
