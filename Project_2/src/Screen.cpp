#include "../include/Ball.hpp"

std::mutex Screen::mtx_;

Screen::Screen() : balls_amount_{0, 0, 0}
{
    initscr();
    cbreak();
    curs_set(FALSE);
    for (size_t i = 0; i < win_number_; ++i)
    {
        main_window_.push_back(Window(75, 25, i));
    }
}

int Screen::get_max_balls() const
{
    return max_balls_;
}

void Screen::increment_balls_amount(const int window_index)
{
    std::lock_guard lg_(mtx_);
    balls_amount_[window_index]++;
}

void Screen::decrement_balls_amount(const int window_index)
{
    std::lock_guard lg_(mtx_);
    balls_amount_[window_index]--;
}

int Screen::get_balls_amount(const int window_index) const
{
    std::lock_guard lg_(mtx_);
    return balls_amount_[window_index];
}

int Screen::get_main_window_size() const
{
    std::lock_guard lg_(mtx_);
    return balls_amount_.size();
}

Window &Screen::get_window(int window_index)
{
    std::lock_guard lg_(mtx_);
    return main_window_[window_index];
}

Screen::~Screen()
{
    endwin();
}