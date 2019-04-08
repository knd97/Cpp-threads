#include "Screen.hpp"

Screen::Screen(int width, int height):
                width_{width}, height_{height}, exit_{false}
{
    initscr();
    cbreak();
    curs_set(FALSE);   
    draw_window();
}

void Screen::calc_window_size()
{
    for(size_t i = 0; i < main_window_.size(); ++i)
    {
        main_window_[i] = newwin(height_, static_cast<int>(width_ / 3), get_center_y(), get_center_x() 
                                + (i * static_cast<int>(width_ / 3)));
    }
}

void Screen::draw_window()
{
    calc_window_size();
    refresh();
    for(size_t i = 0; i < main_window_.size(); ++i)
    {
        box(main_window_.at(i), 0, 0);
        wrefresh(main_window_.at(i));
    }
}

unsigned Screen::rand_window()
{
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist(0, main_window_.size() - 1);

    return dist(mt);
}

void Screen::launch_balls()
{
    screen_thread_ = std::thread(&Screen::check_if_quit, this);

    while(!exit_.load()){
        balls_on_screen_.push_back(std::make_unique<Ball>(main_window_.at(rand_window())));
        balls_on_screen_.back()->th_start();
        wait(std::chrono::milliseconds(5000));
    }
}

void Screen::wait(std::chrono::milliseconds period)
{
    auto start{ std::chrono::system_clock::now() };
    std::chrono::system_clock::time_point end{};

    while(std::chrono::duration_cast<std::chrono::milliseconds>(end - start) < period && !exit_.load())
    {
        end = std::chrono::system_clock::now();
    }
}
void Screen::check_if_quit()
{
    while(!exit_.load())
    {
        if(static_cast<int>(getch()) == 27)
        {
            exit_.store(true);
        }
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
    screen_thread_.join();
    for(auto &ball : balls_on_screen_)
    {
        ball->th_stop();
    }
    balls_on_screen_.clear();
    endwin();
}