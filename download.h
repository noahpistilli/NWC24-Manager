#pragma once

#include "tui.h"
#include "nwc24dl.h"

class DownloadModal : protected TUI {
public:
    DownloadModal() = default;
    ~DownloadModal() override = default;
    void Start() override;

    static void Init();
    static DownloadModal& GetInstance() { return *s_instance; }

private:
    void Download();
    bool ReloadList();

    static DownloadModal* s_instance;
    NWC24Dl m_nwc24dl{};
};