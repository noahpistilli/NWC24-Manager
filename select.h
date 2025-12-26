#pragma once

#include "tui.h"
#include <vector>

class SelectModal : protected TUI {
public:
    SelectModal();
    ~SelectModal() override = default;

    [[noreturn]] void Start() override;

private:
    std::vector<std::string_view> m_options{
      "Download",
      "Stats"
    };
};