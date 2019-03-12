#include "Screen.hpp"

Screen::Screen(int width, int height):
                width_{width}, height_{height}
{
    initscr();
    cbreak();
    curs_set(FALSE);   
    main_window_ = newwin(height_, width_, get_center_y(), get_center_x());
    refresh();
    box(main_window_, 0, 0);
    wrefresh(main_window_);
}

void Screen::launch_balls()
{
    while(true){
        balls_on_screen_.push_back(std::unique_ptr<Ball>(new Ball(main_window_)));
        balls_on_screen_.back()->th_start();
        std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    }
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
    for(auto &ball : balls_on_screen_)
    {
        ball->th_stop();
    }
    endwin();
}