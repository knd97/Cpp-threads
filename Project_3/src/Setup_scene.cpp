#include "../include/Setup_scene.hpp"

Setup_scene::Setup_scene(double frequency, double weather) : exit_{false},
                                                             new_ship_freq_{frequency},
                                                             weather_{weather},
                                                             period_{std::chrono::milliseconds(8000)}

{
    main_window_ = std::make_shared<Window>();
    ramps_ = std::make_shared<SeaPort>(main_window_->get_height(), main_window_->get_width());
}

void Setup_scene::launch_seaport()
{
    main_window_->draw_scene();
    auto i{0};
    //screen_thread_ = std::thread([&]() { check_if_quit(); });

    launch_workers();
    while (!exit_.load())
    {
        ships_.push_back(std::make_unique<Ship>(std::make_pair(20 - i * 2, 40), main_window_, ramps_));
        ships_.back()->start();
        std::this_thread::sleep_for(std::chrono::milliseconds(3000));
        ++i;
    }
}

void Setup_scene::launch_workers()
{
    for (size_t i = 0; i < 1; ++i)
    {
        workers_.push_back(std::make_unique<Worker>(std::make_pair(40 - i * 2, 160), ramps_, main_window_));
        workers_.back()->start();
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

Setup_scene::~Setup_scene()
{
    screen_thread_.join();
    for (auto &ship : ships_)
        ship->stop();

    for (auto &worker : workers_)
        worker->stop();

    ships_.clear();
    workers_.clear();
}