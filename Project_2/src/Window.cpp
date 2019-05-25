#include "Window.hpp"

std::mutex Window::mtx_;

Window::Window(int width, int height, int index) : width_{width}, height_{height},
                                                   window_(newwin(height_, static_cast<int>(width_ / 3),
                                                                  get_center_y(),
                                                                  get_center_x() + (index * static_cast<int>(width_ / 3) - 1)),
                                                           [](WINDOW *w) {
                                                               delwin(w);
                                                               endwin();
                                                           })
{
    refresh();
    box(window_.get(), 0, 0);
    wrefresh(window_.get());
}

void Window::repaint(std::pair<int, int> previous_position, std::pair<int, int> next_position)
{
    std::lock_guard lg(mtx_);
    mvwprintw(window_.get(), previous_position.second, previous_position.first, " ");
    mvwprintw(window_.get(), next_position.second, next_position.first, "o");
    wrefresh(window_.get());
}

void Window::erase_ball(std::pair<int, int> position)
{
    std::lock_guard lg(mtx_);
    mvwprintw(window_.get(), position.second, position.first, " ");
    wrefresh(window_.get());
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
    delwin(window_.get());
}