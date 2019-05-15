#include "Window.hpp"

Window::Window(uint8_t width, uint8_t height, uint8_t index) : width_{width}, height_{height}
{
    initscr();
    cbreak();
    curs_set(FALSE);
    draw_window(index);
}

void Window::draw_window(uint8_t index)
{
    calc_window_size(index);
    refresh();
    box(window_, 0, 0);
    wrefresh(window_);
}

void Window::repaint(std::pair<int, int> previous_position, std::pair<int, int> next_position)
{
    std::lock_guard lg(mtx_);
    mvwprintw(window_, previous_position.second, previous_position.first, " ");
    mvwprintw(window_, next_position.second, next_position.first, "o");
    wrefresh(window_);
}

void Window::erase_ball(std::pair<int, int> position)
{
    std::lock_guard lg(mtx_);
    mvwprintw(window_, position.second, position.first, " ");
    wrefresh(window_);
}

int Window::get_center_x()
{
    return static_cast<int>((COLS - width_) / 2);
}

int Window::get_center_y()
{
    return static_cast<int>((LINES - height_) / 2);
}

void Window::calc_window_size(uint8_t index)
{
    window_ = newwin(height_, static_cast<int>(width_ / 3), get_center_y(),
                     get_center_x() + (index * static_cast<int>(width_ / 3) - 1));
}

WINDOW *Window::get_window()
{
    return window_;
}

void Window::set_window(WINDOW *win)
{
    window_ = win;
}

Window::~Window()
{
    endwin();
}