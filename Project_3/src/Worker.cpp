#include "../include/Worker.hpp"

const std::chrono::milliseconds Worker::speed_ = std::chrono::milliseconds(100);

Worker::Worker(std::pair<int, int> coordinates, std::shared_ptr<SeaPort> ramps, std::shared_ptr<Window> main_window) : starting_poit_{coordinates},
                                                                                                                       seaport_{std::move(ramps)},
                                                                                                                       window_{std::move(main_window)},
                                                                                                                       coordinates_{coordinates}
{
}

void Worker::start()
{
    worker_thread_ = std::thread([&]() { th_func(); });
}

void Worker::th_func()
{
    //while (!stop_thread_.load())
    {
        window_->move_worker(previous_coordinates_, coordinates_);
        worker_to_ramp(seaport_->get_ramp(seaport_->get_free_ramp()));
        back_to_queue();
    }
}

void Worker::stop()
{
    stop_thread_.store(true);
}

void Worker::repaint_worker()
{
    window_->move_worker(previous_coordinates_, coordinates_);
}

void Worker::worker_to_ramp(std::shared_ptr<Ramp> ramp)
{
    auto ramp_coordainates{ramp->get_worker_coords()};
    auto half_way{(ramp_coordainates.second + coordinates_.second) / 2};

    while (coordinates_.second > half_way)
    {
        new_position(std::make_pair(0, -1));
        repaint_worker();
        std::this_thread::sleep_for(speed_);
    }

    while (coordinates_.first != ramp_coordainates.first)
    {
        if (coordinates_.first - ramp_coordainates.first < 0)
            new_position(std::make_pair(1, 0));
        else
            new_position(std::make_pair(-1, 0));
        repaint_worker();
        std::this_thread::sleep_for(speed_);
    }

    while (coordinates_.second > ramp_coordainates.second)
    {
        new_position(std::make_pair(0, -1));
        repaint_worker();
        std::this_thread::sleep_for(speed_);
    }
}

void Worker::new_position(std::pair<int, int> step)
{
    previous_coordinates_ = coordinates_;
    coordinates_.first += step.first;
    coordinates_.second += step.second;
}

void Worker::back_to_queue()
{
    auto half_way{(coordinates_.second + starting_poit_.second) / 2};

    while (coordinates_.second < half_way)
    {
        new_position(std::make_pair(0, 1));
        repaint_worker();
        std::this_thread::sleep_for(speed_);
    }

    while (coordinates_.first != starting_poit_.first)
    {
        if (coordinates_.first - starting_poit_.first < 0)
            new_position(std::make_pair(1, 0));
        else
            new_position(std::make_pair(-1, 0));
        repaint_worker();
        std::this_thread::sleep_for(speed_);
    }

    while (coordinates_.second < starting_poit_.second)
    {
        new_position(std::make_pair(0, 1));
        repaint_worker();
        std::this_thread::sleep_for(speed_);
    }
}

Worker::~Worker()
{
    worker_thread_.join();
}