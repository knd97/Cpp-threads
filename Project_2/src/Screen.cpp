#include "Screen.hpp"

std::mutex Screen::mtx_;

Screen::Screen() : balls_amount_{0, 0, 0}
{
    initscr();
    cbreak();
    curs_set(FALSE);
    for (size_t i = 0; i < 3; ++i)
    {
        main_window_.push_back(Window(75, 25, i));
    }
}

int Screen::get_max_balls() const
{
    return max_balls_;
}

void Screen::increment_balls(const int index)
{
    std::lock_guard lg_(mtx_);
    balls_amount_[index]++;
}

void Screen::decrement_balls(const int index)
{
    std::lock_guard lg_(mtx_);
    balls_amount_[index]--;
}

int Screen::get_amount(const int index)
{
    std::lock_guard lg_(mtx_);
    return balls_amount_[index];
}

int Screen::get_main_window_size() const
{
    std::lock_guard lg_(mtx_);
    return balls_amount_.size();
}

Window &Screen::get_window(int index)
{
    std::lock_guard lg_(mtx_);
    return main_window_[index];
}

Screen::~Screen()
{
    endwin();
}