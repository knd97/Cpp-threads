#include "../include/Ball.hpp"

std::mutex Ball::m_ball_;
std::condition_variable Ball::c_v_;
std::atomic<bool> Ball::possible_move_ = false;
std::random_device Ball::rd_;
std::mt19937 Ball::mt_(Ball::rd_());
const std::chrono::milliseconds Ball::interval_ = std::chrono::milliseconds(40);
const std::chrono::seconds Ball::lifetime_ = std::chrono::seconds(5);
const std::vector<std::pair<int, int>> Ball::possible_directory_ = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 1}, {-1, 1}, {-1, -1}, {1, -1}};

Ball::Ball(int index, std::shared_ptr<Screen> scr) : screen_{std::move(scr)},
                                                     window_index_{index},
                                                     stop_thread_{false}
{
    coordinates_ = std::make_pair(screen_->get_window(index).get_maxx(),
                                  screen_->get_window(index).get_maxy());
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
    possible_move_.store(true);
    c_v_.notify_all();
}

void Ball::th_func()
{
    auto counter{1};
    while (!stop_thread_.load())
    {
        move();
        if (counter % 20 == 0)
        {
            transit();
        }
        std::this_thread::sleep_for(interval_);
        ++counter;
    }
    screen_->get_window(window_index_).erase_ball(position_);
}

void Ball::transit()
{
    std::unique_lock lck(m_ball_);
    c_v_.wait_for(lck, lifetime_, [&]() { return possible_move_.load(); });

    if (possible_move_.load())
    {
        lck.unlock();
        possible_move_.store(false);
        c_v_.notify_all();
        if (screen_->get_balls_amount(next_win_index_) < screen_->get_max_balls())
        {
            screen_->decrement_balls_amount(window_index_);
            screen_->get_window(window_index_).erase_ball(position_);
            set_next_wind();
            screen_->increment_balls_amount(window_index_);
        }
    }
    else
    {
        lck.unlock();
        screen_->decrement_balls_amount(window_index_);
        th_stop();
    }
}

void Ball::move()
{
    auto current_position{position_};
    new_position();
    screen_->get_window(window_index_).repaint_ball(current_position, position_);
    check_free_window();
}

void Ball::set_next_wind()
{
    std::lock_guard lg_(m_ball_);
    window_index_ = next_win_index_;
}

std::pair<int, int> Ball::random_direction() const
{
    std::uniform_int_distribution<int> dist(0, possible_directory_.size() - 1);

    return possible_directory_[dist(mt_)];
}

void Ball::check_free_window()
{
    auto break_{false};
    for (size_t i = 0; i < 3; ++i)
    {
        if (screen_->get_balls_amount(i) < screen_->get_max_balls())
        {
            possible_move_.store(true);
            next_win_index_ = i;
            break_ = true;
        }
    }
    if (!break_)
    {
        possible_move_.store(false);
    }
}

void Ball::check_if_rebound()
{
    if (position_.first + coordinates_.first >= screen_->get_window(window_index_).get_maxx() - 1 ||
        position_.first + coordinates_.first <= 1)
    {
        coordinates_.first *= -1;
    }
    else if (position_.second + coordinates_.second >= screen_->get_window(window_index_).get_maxy() - 1 ||
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