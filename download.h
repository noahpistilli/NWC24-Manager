#pragma once

#include "tui.h"
#include "nwc24dl.h"

class DownloadModal : public TUI {
public:
    DownloadModal();
    void Start() override;
    void Download();

private:
    NWC24Dl* m_nwc24_dl = nullptr;
};