#include "Ball.hpp"

std::chrono::milliseconds Ball::interval_ = std::chrono::milliseconds(40);
const char *Ball::ball_symbol_ = "0";
std::mutex m_ball_;

Ball::Ball(WINDOW * window): 
            ball_thread_(), window_{ window }, stop_thread_{false}
{
    getmaxyx(window, coordinates_.second, coordinates_.first);
    coordinates_ = {coordinates_.first / 2, coordinates_.second / 2};
    mvwprintw(window, coordinates_.second, coordinates_.first, ball_symbol_);
    wrefresh(window);
}

void Ball::th_func()
{
   while(!stop_thread_)
   {
       move();                                                                      //move and repaint
       std::this_thread::sleep_for(interval_);
   }
}

void Ball::th_start()
{
    ball_thread_ = std::thread(&Ball::th_func, this);
}

void Ball::th_stop()
{
    stop_thread_ = true;
}

void Ball::move()
{
    auto current_position {coordinates_};
    new_coordinates(random_direction());
    repaint_ball(current_position);
}

std::pair<int, int> Ball::random_direction() const
{
    std::random_device rd;
    std::mt19937 mt(rd());                                                               //Mersenne Twister engine
    std::uniform_int_distribution<int> dist(0, 7);

    return possible_moves_.at(dist(mt));
}

void Ball::repaint_ball(std::pair<int, int> previous_position)
{
    std::lock_guard<std::mutex> lg(m_ball_);
    mvwprintw(window_, previous_position.second, previous_position.first, " ");
    mvwprintw(window_, coordinates_.second, coordinates_.first, ball_symbol_);
    wrefresh(window_);
}

void Ball::new_coordinates(std::pair<int, int> direction)
{
    if(coordinates_.first + direction.first >= getmaxx(window_) - 1 || coordinates_.first + direction.first <= 1)                  //x >= window_size || x <= 1                    
    {
        direction.first *= -1;
    }
    else if(coordinates_.second + direction.second >= getmaxy(window_) - 1 || coordinates_.second + direction.second <= 1)
    {
        direction.second *= -1;
    }
    coordinates_.first += direction.first;
    coordinates_.second += direction.second;
}

Ball::~Ball()
{
    stop_thread_ = true;
    if(ball_thread_.joinable())
    {
        ball_thread_.join();
    }
}