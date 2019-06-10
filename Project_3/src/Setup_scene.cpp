#include "../include/Setup_scene.hpp"

Setup_scene::Setup_scene(double frequency, double weather) : exit_{false},
                                                             new_ship_freq_{frequency},
                                                             weather_{weather},
                                                             period_{std::chrono::milliseconds(15000)}

{
    main_winodw_ = std::make_shared<Window>();
    main_winodw_->draw_scene();
    ramps_ = std::make_shared<SeaPort>(main_winodw_->get_height(), main_winodw_->get_width());
}

void Setup_scene::launch_seaport()
{
    auto i{0};
    screen_thread_ = std::thread([&]() { check_if_quit(); });

    //launch_workers();
    while (!exit_.load())
    {
        ships_.push_back(std::make_unique<Ship>(std::make_pair(40 - i * 2, 40), main_winodw_, ramps_));
        ships_.back()->start();
        workers_.push_back(std::make_unique<Worker>(std::make_pair(40 - i * 2, 160), ramps_, main_winodw_));
        workers_.back()->start();
        wait();
        ++i;
    }
}

void Setup_scene::launch_workers()
{
    //for (size_t i = 0; i < 1; ++i)
    //{
    //    workers_.push_back(std::make_unique<Worker>(std::make_pair(160, 40 - i * 2), ramps_, main_winodw_));
    //    workers_.back()->start();
    //}
}

void Setup_scene::check_if_quit()
{
    while (!exit_.load())
    {
        if (static_cast<int>(getch()) == 27)
            exit_.store(true);
    }
}

void Setup_scene::wait()
{
    auto start{std::chrono::system_clock::now()};
    std::chrono::system_clock::time_point end{};

    while (std::chrono::duration_cast<std::chrono::milliseconds>(end - start) < period_ * new_ship_freq_ && !exit_.load())
    {
        end = std::chrono::system_clock::now();
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