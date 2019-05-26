#include "../include/Ball.hpp"

std::mutex Screen::mtx_;
std::random_device Screen::rd_;
std::mt19937 Screen::mt_(Screen::rd_());

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

int Screen::get_free_window()
{
    std::vector<int> target_;
    for (size_t i = 0; i < win_number_; ++i)
    {
        if (balls_amount_[i] < max_balls_)
            target_.push_back(i);
    }

    if (target_.size() == 0)
    {
        return -1;
    }
    else
    {
        std::uniform_int_distribution<int> dist(0, target_.size() - 1);
        return target_[dist(mt_)];
    }
}

Screen::~Screen()
{
    endwin();
}