#include "../include/Setup_scene.hpp"

std::random_device Setup_scene::rd_;
std::mt19937 Setup_scene::mt_(Setup_scene::rd_());

Setup_scene::Setup_scene() : screen_{std::make_shared<Screen>()},
                             exit_{false}
{
}

void Setup_scene::launch_balls()
{
    screen_thread_ = std::thread([&]() { check_if_quit(); });

    while (!exit_.load())
    {
        auto win_number{random_window_index()};
        if (screen_->get_balls_amount(win_number) < screen_->get_max_balls())
        {
            screen_->increment_balls_amount(win_number);
            balls_on_screen_.push_back(std::make_unique<Ball>(win_number, screen_));
            balls_on_screen_.back()->th_start();
            wait(std::chrono::milliseconds(3000));
        }
    }
}

void Setup_scene::wait(std::chrono::milliseconds period)
{
    auto start{std::chrono::system_clock::now()};
    std::chrono::system_clock::time_point end{};

    while (std::chrono::duration_cast<std::chrono::milliseconds>(end - start) < period && !exit_.load())
    {
        end = std::chrono::system_clock::now();
    }
}

void Setup_scene::check_if_quit()
{
    while (!exit_.load())
    {
        if (static_cast<int>(getch()) == 27)
            exit_.store(true);
    }
}

int Setup_scene::random_window_index()
{
    std::uniform_int_distribution<int> dist(0, screen_->get_main_window_size() - 1);

    return dist(mt_);
}

Setup_scene::~Setup_scene()
{
    screen_thread_.join();
    for (auto &ball : balls_on_screen_)
        ball->th_stop();

    balls_on_screen_.clear();
}