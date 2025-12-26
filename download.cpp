#include "download.h"
#include "nwc24.h"
#include <iostream>
#include <sys/unistd.h>


void DownloadModal::Start() {
  bool success = ReloadList();
  if (!success) {
    WII_ReturnToMenu();
  }

  auto urls = m_nwc24dl.GetDownloadURLs();
  auto ids = m_nwc24dl.GetGameIDs();

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
  ClearScreen();
  PrintTopBar("Downloading File");

  // It is possible that KD modified the list during the time it took the user to navigate to the task
  // and select it. Reload every time.
  bool success = ReloadList();
  if (!success) {
    WII_ReturnToMenu();
  }


  void *io_buf = std::malloc(32);
  void *in_buf = std::malloc(32);

  u16 _index = m_nwc24dl.GetIndex(m_index);
  u32 flags = m_nwc24dl.GetFlags(_index);
  u32 bitmask = m_nwc24dl.GetBitMask(_index);
  *(static_cast<u32*>(in_buf)) = flags;
  *(static_cast<u16*>(in_buf) + 3) = _index;
  *(static_cast<u32*>(in_buf) + 2) = bitmask;

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

  const s32 response = static_cast<s32 *>(io_buf)[0];
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
      success = ReloadList();
      if (!success) {
        WII_ReturnToMenu();
      }

      return;
    }
  }
}

bool DownloadModal::ReloadList() {
  bool success = m_nwc24dl.ReadDlList();
  if (!success) {
    std::cout << "Failed to read DL list" << std::endl;
    sleep(5);
    return false;
  }

  m_num_of_lines = m_nwc24dl.GetNumberOfEntries();
  return true;
}
