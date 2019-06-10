#include "../include/Worker.hpp"

void Worker::start()
{
    std::thread([&]() { th_func(); });
}

void Worker::th_func()
{
    while (!stop_thread_.load())
    {
        worker_to_ramp();
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

    while (coordinates_.second < half_way)
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
        new_position(std::make_pair(-1, 0));
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
}

Worker::~Worker()
{
    worker_thread_.join();
}