#include "Screen.hpp"

Screen::Screen(int width, int height):
                width_{width}, height_{height}
{
    initscr();
    cbreak();    
    main_window_ = newwin(height_, width_, get_center_y(), get_center_x());
    refresh();
    box(main_window_, 0, 0);
    wprintw(main_window_, "Konrad Olszewski 238898");
    wrefresh(main_window_);
}

int Screen::get_center_x()
{
    return static_cast<int>((COLS - width_) / 2);
}

int Screen::get_center_y()
{
    return static_cast<int>((LINES - height_) / 2);
}

Screen::~Screen()
{
    endwin();
}