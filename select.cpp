#include "select.h"
#include <iostream>
#include "download.h"

SelectModal::SelectModal() {
  m_num_of_lines = static_cast<int>(m_options.size());
}

[[noreturn]] void SelectModal::Start() {
  for (int i = 0; true; i++) {
    if (i != 0 || m_num_of_lines != 1) {
      ClearScreen();
      PrintTopBar("Pick an option");

      int printable = MIN(25 - OVERSCAN_Y_TIMES_2, m_num_of_lines);
      for (int j = m_offset; j < (m_offset + printable); j++) {
        PrintCursor(j);
        std::cout << m_options.at(j) << std::endl;
      }

      PrintBottomBar("Press A to select an item");
    }

    Action ret = (i == 0 && m_num_of_lines == 1) ? Action::A : ProcessInputs(true);

    if (ret == Action::Home) {
      WII_ReturnToMenu();
    } else if (ret == Action::A) {
      auto download = DownloadModal();
      download.Start();
    }
  }

  __builtin_unreachable();
}