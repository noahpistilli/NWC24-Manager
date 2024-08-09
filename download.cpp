#include "download.h"
#include "nwc24.h"
#include <iostream>


DownloadModal::DownloadModal() {
  m_nwc24_dl = new NWC24Dl();
  m_num_of_lines = static_cast<int>(m_nwc24_dl->GetDownloadURLs().size());
}

void DownloadModal::Start() {
  auto urls = m_nwc24_dl->GetDownloadURLs();
  auto ids = m_nwc24_dl->GetGameIDs();
  auto indexes = m_nwc24_dl->GetIndexes();

  for (int i = 0; true; i++) {
    if (i != 0 || m_num_of_lines != 1) {
      ClearScreen();
      PrintTopBar("Pick a channel to download");

      int printable = MIN(25 - OVERSCAN_Y_TIMES_2, m_num_of_lines);
      for (int j = m_offset; j < (m_offset + printable); j++) {
        PrintCursor(j);
        std::cout << ids.at(j) << ": " << urls.at(j) << std::endl;
      }

      PrintBottomBar("Press A to select an item");
    }

    Action ret = (i == 0 && m_num_of_lines == 1) ? Action::A : ProcessInputs(true);

    if (ret == Action::A) {
      Download();
    } else if (ret == Action::B) {
      return;
    }
  }
}

void DownloadModal::Download() {
  auto ids = m_nwc24_dl->GetGameIDs();
  auto indexes = m_nwc24_dl->GetIndexes();

  ClearScreen();
  PrintTopBar("Downloading File");
  std::cout << "Downloading " << ids.at(m_index) << std::endl;

  void *io_buf = std::malloc(32);
  void *in_buf = std::malloc(32);

  u16 _index = indexes.at(m_index);
  u32 flags = m_nwc24_dl->GetFlags(_index);
  u32 bitmask = m_nwc24_dl->GetBitMask(_index);
  *(reinterpret_cast<u32*>(in_buf)) = flags;
  *(reinterpret_cast<u16*>(in_buf) + 3) = _index;
  *(reinterpret_cast<u32*>(in_buf) + 2) = bitmask;

  s32 _ret = NWC24::GetInstance().DoIoctl(NWC24::Command::DownloadNow, 32, in_buf, 32, io_buf);
  if (_ret < 0) {
    std::cout << "A fatal error has occurred in the WC24 device." << std::endl;
    PrintBottomBar("Press B to return item");

    while (true) {
      Action ret = ProcessInputs(true);
      if (ret == Action::B) {
        return;
      }
    }
  }

  const s32 response = reinterpret_cast<s32 *>(io_buf)[0];
  if (response != 0)
  {
    const s32 wc24_error = NWC24::GetInstance().GetLastError();
    std::cout << "An error has occurred in the patching process." << std::endl;
    std::cout << wc24_error << std::endl;
    PrintBottomBar("Press B to return");

    while (true) {
      Action ret = ProcessInputs(true);
      if (ret == Action::B) {
        return;
      }
    }
  }

  std::cout << "Downloaded!" << std::endl;

  PrintBottomBar("Press B to return item");

  while (true) {
    Action ret = ProcessInputs(true);
    if (ret == Action::B) {
      return;
    }
  }
}