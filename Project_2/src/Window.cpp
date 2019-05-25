#include "Window.hpp"

std::mutex Window::mtx_;

Window::Window(int width, int height, int index) : width_{width}, height_{height}
{
    draw_window(index);
}

void Window::draw_window(int index)
{
    create_window(index);
    refresh();
    box(window_, 0, 0);
    wrefresh(window_);
}

void Window::create_window(int index)
{
    window_ = newwin(height_, static_cast<int>(width_ / 3), get_center_y(),
                     get_center_x() + (index * static_cast<int>(width_ / 3) - 1));
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

int Window::get_maxx() const
{
    return window_->_maxx;
}

int Window::get_maxy() const
{
    return window_->_maxy;
}

int Window::get_center_x()
{
    return static_cast<int>((COLS - width_) / 2);
}

int Window::get_center_y()
{
    return static_cast<int>((LINES - height_) / 2);
}

Window::~Window()
{
    delwin(window_);
}