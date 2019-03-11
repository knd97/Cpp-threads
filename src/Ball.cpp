#include "Ball.hpp"

const char *Ball::ball_symbol_ = "o";
std::chrono::milliseconds Ball::interval_ = std::chrono::milliseconds(20);

Ball::Ball() : ball_thread_()
{
}

Ball::Ball(WINDOW * window): ball_thread_()
{
    stop_thread_ = false;
    getmaxyx(window, coordinates_.second, coordinates_.first);
    mvwprintw(window, coordinates_.second, coordinates_.first, ball_symbol_);
    wrefresh(window);
}

void Ball::th_func()
{
    while(!stop_thread_)
    {
        //move();                                                                  //move and repaint
        std::this_thread::sleep_for(interval_);
    }
}

void Ball::th_start()
{
    ball_thread_ = std::thread(&Ball::th_func, this);
}

Ball::~Ball()
{
    stop_thread_ = true;
    if(ball_thread_.joinable())
    {
        ball_thread_.join();
    }
}