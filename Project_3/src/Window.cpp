#include "../include/Window.hpp"

Window::Window() : width_{getmaxx(stdscr)},
                   height_{getmaxy(stdscr)},
                   window_(newwin(getmaxy(stdscr), getmaxx(stdscr), 0, 0),
                           [](WINDOW *w) {
                               delwin(w);
                               endwin();
                           })
{
    initscr();
    cbreak();
    curs_set(FALSE);
    refresh();
    box(window_.get(), 0, 0);
    wrefresh(window_.get());
    draw_scene();
}

void Window::draw_scene()
{
    init_pairs();
    draw_ramps();
}

void Window::draw_ramps()
{
    for (size_t i = 0; i < 3; ++i)
    {
        for (size_t j = 0; j < 7; ++j)
        {
            wattron(window_.get(), COLOR_PAIR(RED));
            mvwprintw(window_.get(), height_ / 2 - 5 + i * 3, width_ / 2 + j, "-");
            wattroff(window_.get(), COLOR_PAIR(BLUE));
        }
        mvprintw(height_ / 2 - 5 + i * 3 + 1, width_ / 2 + 3, "|");
        mvprintw(height_ / 2 - 5 + i * 3 + 2, width_ / 2 + 3, "|");
    }
    for (size_t j = 0; j < 7; ++j)
    {
        mvprintw(height_ / 2 - 5 + 3 * 3, width_ / 2 + j, "-");
    }
    wrefresh(window_.get());
}

void Window::init_pairs()
{
    start_color();
    init_color(COLOR_GREEN, 0, 700, 0);
    init_color(COLOR_RED, 700, 0, 0);

    init_pair(WHITE, COLOR_BLACK, COLOR_BLACK);
    init_pair(RED, COLOR_RED, -1);
    init_pair(BLUE, COLOR_BLUE, -1);
    init_pair(GREEN, COLOR_GREEN, COLOR_BLACK);
    init_pair(PINK, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(YELLOW, COLOR_YELLOW, COLOR_BLACK);
}

Window::~Window()
{
    endwin();
}