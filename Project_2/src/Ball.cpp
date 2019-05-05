#include "Ball.hpp"

std::mutex m_ball_;

Ball::Ball(WINDOW *window) : ball_thread_(), window_{window},
                             stop_thread_{false}, ended_{false}
{
    getmaxyx(window, coordinates_.second, coordinates_.first);
    position_ = {coordinates_.first / 2, coordinates_.second / 2};
    coordinates_ = {random_direction()};
    mvwprintw(window, position_.second, position_.first, ball_symbol_);
    wrefresh(window);
}

void Ball::th_func()
{
    auto start{std::chrono::system_clock::now()};
    std::chrono::system_clock::time_point end{};

    while (std::chrono::duration_cast<std::chrono::milliseconds>(end - start) < lifetime_ && !stop_thread_.load())
    {
        move();
        std::this_thread::sleep_for(interval_);
        end = std::chrono::system_clock::now();
    }
    erase_ball();
    th_stop();
}

void Ball::set_new_window(WINDOW *new_win)
{
    erase_ball();
    this->window_ = new_win;
}

void Ball::th_start()
{
    ball_thread_ = std::thread(&Ball::th_func, this);
}

void Ball::th_stop()
{
    stop_thread_.store(true);
}

void Ball::erase_ball()
{
    std::lock_guard<std::mutex> lg(m_ball_);
    mvwprintw(window_, position_.second, position_.first, " ");
    wrefresh(window_);
}

void Ball::move()
{
    auto current_position{position_};
    new_position();
    repaint_ball(current_position);
}

std::pair<int, int> Ball::random_direction() const
{
    std::random_device rd;
    std::mt19937 mt(rd()); //Mersenne Twister engine
    std::uniform_int_distribution<int> dist(0, 7);

    return possible_moves_[dist(mt)];
}

WINDOW *Ball::get_window() const
{
    return window_;
}

void Ball::set_ended()
{
    ended_.store(true);
}

bool Ball::check_if_ended()
{
    return ended_.load();
}

void Ball::repaint_ball(std::pair<int, int> previous_position)
{
    std::lock_guard<std::mutex> lg(m_ball_);
    mvwprintw(window_, previous_position.second, previous_position.first, " ");
    mvwprintw(window_, position_.second, position_.first, ball_symbol_);
    wrefresh(window_);
}

bool Ball::check_status()
{
    return stop_thread_.load();
}

void Ball::check_if_rebound()
{
    if (position_.first + coordinates_.first >= getmaxx(window_) - 1 ||
        position_.first + coordinates_.first <= 1)
    {
        coordinates_.first *= -1;
    }
    else if (position_.second + coordinates_.second >= getmaxy(window_) - 1 ||
             position_.second + coordinates_.second <= 1)
    {
        coordinates_.second *= -1;
    }
}

void Ball::new_position()
{
    check_if_rebound();
    position_.first += coordinates_.first;
    position_.second += coordinates_.second;
}

Ball::~Ball()
{
    ball_thread_.join();
}