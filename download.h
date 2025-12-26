#pragma once

#include "tui.h"
#include "nwc24dl.h"

class DownloadModal : protected TUI {
public:
    DownloadModal() = default;
    ~DownloadModal() override = default;
    void Start() override;

private:
    void Download();
    bool ReloadList();

    NWC24Dl m_nwc24dl{};
};