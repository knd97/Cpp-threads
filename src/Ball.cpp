#include "Ball.hpp"

Ball::~Ball()
{
    ball_thread_.join();
}