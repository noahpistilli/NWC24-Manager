#pragma once

#include <string>
#include <iostream>

#define OVERSCAN_X 3
#define OVERSCAN_X_SPACES "   "

#define OVERSCAN_Y 2
#define OVERSCAN_Y_LINES_MINUS_1 "\n"

#define LINE "============================================================================="

#define OVERSCAN_Y_LINES OVERSCAN_Y_LINES_MINUS_1 "\n"
#define OVERSCAN_Y_TIMES_2 (OVERSCAN_Y * 2)


enum class Action {
    Home,
    A,
    B,
    None,
};

class TUI {
protected:
    virtual void Start() {};

    Action ProcessInputs(bool scroll);

    static void PrintTopBar(std::string_view msg);
    static void PrintBottomBar(std::string_view msg);
    void PrintCursor(int cur) const;
    static void ClearScreen();

    int m_num_of_lines{};
    int m_offset = 0;
    int m_index = 0;

};