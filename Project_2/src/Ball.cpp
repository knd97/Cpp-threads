#include "Ball.hpp"

std::mutex Ball::m_ball_;
std::condition_variable Ball::c_v_;
std::atomic<bool> Ball::possible_move_ = false;
uint8_t Ball::next_win_index_ = 0;

Ball::Ball(std::unique_ptr<Window> &win, uint8_t index, Screen &scr) : screen_{scr}, window_{win},
                                                                       window_index_{index},
                                                                       stop_thread_{false}, ended_{false}
{
    getmaxyx(window_->get_window(), coordinates_.second, coordinates_.first);
    position_ = {coordinates_.first / 2, coordinates_.second / 2};
    coordinates_ = {random_direction()};
}

void Ball::th_start()
{
    ball_thread_ = std::thread([&]() { th_func(); });
}

void Ball::th_stop()
{
    stop_thread_.store(true);
    c_v_.notify_all();
}

void Ball::th_func()
{
    while (!stop_thread_.load())
    {
        move();
        std::this_thread::sleep_for(interval_);
    }
    window_->erase_ball(position_);
}

void Ball::transit()
{
    std::unique_lock<std::mutex> lck(m_ball_);
    c_v_.wait_for(lck, lifetime_, [&]() { return possible_move_.load(); });

    if (possible_move_.load())
    {
        }
    else
        th_stop();
}

void Ball::move()
{
    auto current_position{position_};
    new_position();
    window_->repaint(current_position, position_);
    check_free_window();
    //transit();
}

void Ball::set_next_wind(WINDOW *win, uint8_t index)
{
    window_->set_window(win);
    window_index_ = index;
}

std::pair<int, int> Ball::random_direction() const
{
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist(0, possible_directory_.size() - 1);

    return possible_directory_[dist(mt)];
}

void Ball::check_free_window()
{
    for (size_t i = 0; i < screen_.get_array_size(); ++i)
    {
        if (screen_.get_amount(i) < screen_.get_max_balls())
        {
            possible_move_.store(true);
            next_win_index_ = i;
            break;
        }
    }
}

void Ball::end_ball()
{
    ended_.store(true);
}

bool Ball::check_ended()
{
    return ended_.load();
}

bool Ball::check_stop()
{
    return stop_thread_.load();
}

uint8_t Ball::get_win_index()
{
    return window_index_;
}

void Ball::check_if_rebound()
{
    if (position_.first + coordinates_.first >= getmaxx(window_->get_window()) - 1 ||
        position_.first + coordinates_.first <= 1)
    {
        coordinates_.first *= -1;
    }
    else if (position_.second + coordinates_.second >= getmaxy(window_->get_window()) - 1 ||
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