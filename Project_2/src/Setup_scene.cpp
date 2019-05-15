#include "Setup_scene.hpp"

Setup_scene::Setup_scene() : screen_{Screen()}, exit_{false}
{
}

void Setup_scene::launch_balls()
{
    screen_thread_ = std::thread([&]() { check_if_quit(); });

    while (!exit_.load())
    {
        check_balls();
        auto win_number{random_index()};
        if (screen_.get_amount(win_number) < screen_.get_max_balls())
        {
            balls_on_screen_.push_back(std::make_unique<Ball>(screen_.get_window(win_number), win_number, screen_));
            balls_on_screen_.back()->th_start();
            screen_.increment_balls(win_number);
            wait(std::chrono::milliseconds(3000));
        }
    }
}

void Setup_scene::check_balls()
{
    for (auto &it : balls_on_screen_)
    {
        if (it->check_stop() && !it->check_ended())
        {
            it->end_ball();
            screen_.decrement_balls(it->get_win_index());
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

uint8_t Setup_scene::random_index()
{
    //static
    std::random_device rd;
    std::mt19937 mt(rd()); //Mersenne Twister engine
    std::uniform_int_distribution<uint8_t> dist(0, screen_.get_array_size() - 1);

    return dist(mt);
}

Setup_scene::~Setup_scene()
{
    screen_thread_.join();
    for (auto &ball : balls_on_screen_)
        ball->th_stop();

    balls_on_screen_.clear();
}