#include "tui.h"
#include <memory>
#include <gccore.h>
#include <iostream>
#include <wiiuse/wpad.h>

Action TUI::ProcessInputs(bool scroll) {
  int canDisplayAmount = MIN(25 - OVERSCAN_Y_TIMES_2, m_num_of_lines - m_offset);
  int maxOffset = MAX(0, m_num_of_lines - canDisplayAmount);
  int endOfIndex = m_offset + canDisplayAmount;

  while(true) {
    WPAD_ScanPads();

    u32 pressed = WPAD_ButtonsDown(0);
    if (pressed & WPAD_BUTTON_DOWN || (!scroll && (pressed & WPAD_BUTTON_RIGHT))) {
      m_index++;
      if (!scroll)
        m_index = endOfIndex;

      if (m_index >= endOfIndex) {
        m_offset++;

        if (m_offset > maxOffset)
          m_offset--;
      }
      if (m_index >= m_num_of_lines)
        m_index--;

      return Action::None;
    }
    else if (pressed & WPAD_BUTTON_UP || (!scroll && (pressed & WPAD_BUTTON_LEFT))) {
      m_index--;

      if (!scroll)
        m_index = m_offset - 1;

      if (m_index < m_offset) {
        m_offset--;

        if (m_offset < 0)
          m_offset++;
      }
      if (m_index < 0)
        m_index++;

      return Action::None;
    }
    else if (scroll && pressed & WPAD_BUTTON_LEFT) {
      m_index -= canDisplayAmount;
      m_offset -= canDisplayAmount;
      if (m_index < 0)
        m_index= 0;

      if (m_offset < 0)
        m_offset = 0;

      return Action::None;
    }
    else if (scroll && pressed & WPAD_BUTTON_RIGHT) {
      m_index += canDisplayAmount;
      m_offset += canDisplayAmount;

      if (m_index >= m_num_of_lines)
        m_index = m_num_of_lines - 1;

      if (m_offset > maxOffset)
        m_offset = maxOffset;

      return Action::None;
    }

    else if (pressed & WPAD_BUTTON_HOME) {
      return Action::Home;
    }
    else if (pressed & WPAD_BUTTON_A) {
      return Action::A;
    }
    else if (pressed & WPAD_BUTTON_B) {
      return Action::B;
    }
  }

  __builtin_unreachable();
}

void TUI::PrintTopBar(std::string_view msg) {
  std::cout << OVERSCAN_Y_LINES << OVERSCAN_X_SPACES << msg << std::endl << LINE;
}

void TUI::PrintBottomBar(std::string_view msg) {
  std::cout << LINE << OVERSCAN_X_SPACES << msg;
}

void TUI::PrintCursor(int cur) const {
  std::string str(OVERSCAN_X_SPACES);
  if (cur == m_index)
    str[OVERSCAN_X - 2] = '>';

  std::cout << str;
}

void TUI::ClearScreen() {
  std::cout << "\x1b[2J";
}
