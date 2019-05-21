#include "Screen.hpp"

std::mutex Screen::mtx_;

Screen::Screen() : balls_amount_{0, 0, 0}
{
    initscr();
    cbreak();
    curs_set(FALSE);
    for (size_t i = 0; i < main_window_.size(); ++i)
    {
        main_window_[i] = std::make_shared<Window>(75, 25, i);
    }
}

uint8_t Screen::get_max_balls() const
{
    return max_balls_;
}

void Screen::increment_balls(const uint8_t index)
{
    std::lock_guard lg_(mtx_);
    balls_amount_[index]++;
}

void Screen::decrement_balls(const uint8_t index)
{
    std::lock_guard lg_(mtx_);
    balls_amount_[index]--;
}

uint8_t Screen::get_amount(const uint8_t index)
{
    std::lock_guard lg_(mtx_);
    return balls_amount_[index];
}

uint8_t Screen::get_array_size()
{
    return balls_amount_.size();
}

std::shared_ptr<Window> &Screen::get_window(uint8_t index)
{
    return main_window_[index];
}

Screen::~Screen()
{
    endwin();
}