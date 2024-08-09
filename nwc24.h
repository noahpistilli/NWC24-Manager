#pragma once

#include <gccore.h>
#include <array>

class NWC24 final {
public:
    enum class Command : s32 {
        DownloadNow = 14,
        GetSchedulerStat = 30,
    };

    static NWC24& GetInstance() { return *s_instance; }
    static void Init();

    s32 DoIoctl(Command command, s32 in_size, void* in_buf, s32 io_size, void* io_buf) const;
    s32 GetLastError() const;

private:
    NWC24();
    static NWC24* s_instance;

    s32 m_fd{};
};