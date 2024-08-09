#pragma once

#include "tui.h"
#include <vector>

class SelectModal : public TUI {
public:
    SelectModal();

    [[noreturn]] void Start() override;

private:
    std::vector<std::string_view> m_options{
      "Download",
      "Stats"
    };
};