#include "Screen.hpp"

Screen::Screen(int startX, int startY, int width, int height):
                startx_{startX}, starty_{startY}, width_{width}, 
                height_{height}
{
    initscr();
    cbreak();
    main_window = newwin(height_, width_, starty_, startx_);
    refresh();
    box(main_window, 0, 0);
    wprintw(main_window, "Konrad Olszewski 238898");
    wrefresh(main_window);
}

Screen::~Screen()
{
    endwin();
}