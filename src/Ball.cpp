#include "Ball.hpp"

Ball::Ball(WINDOW * widnow, std::chrono::milliseconds period): ball_thread_()
{
    interval_ = period;
    stop_thread_ = false;
    coordinates_.first = 0;
    coordinates_.second = 0;
}

Ball::~Ball()
{
    stop_thread_ = true;
    if(ball_thread_.joinable())
    {
        ball_thread_.join();
    }
}