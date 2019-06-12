#include "../include/Worker.hpp"

const std::chrono::milliseconds Worker::speed_ = std::chrono::milliseconds(100);
std::mutex Worker::m_worker_;
std::random_device Worker::rd_;
std::mt19937 Worker::mt_(Worker::rd_());

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
    while (!stop_thread_.load())
    {
        window_->move_worker(previous_coordinates_, coordinates_);
        std::unique_lock lck(m_worker_);
        cv_workers_.wait(lck, [&]() { return seaport_->check_if_worker_needed(); });

        lck.unlock();
        int occupied_ramp{seaport_->worker_needed()};
        seaport_->worket_to_ramp(occupied_ramp);

        worker_to_ramp(seaport_->get_ramp(occupied_ramp));
        {
            std::unique_lock lck_(seaport_->mtx_unload_);
            unpack_ship();
        }
        seaport_->cv_unload_.notify_one();

        seaport_->worker_finished(occupied_ramp);
        back_to_queue();
        std::this_thread::sleep_for(std::chrono::milliseconds(400));
    }
}

void Worker::stop()
{
    stop_thread_.store(true);
    window_->erase_worker(coordinates_);
    seaport_->cv_unload_.notify_all();
    cv_workers_.notify_all();
}

void Worker::repaint_worker()
{
    window_->move_worker(previous_coordinates_, coordinates_);
}

void Worker::worker_to_ramp(std::shared_ptr<Ramp> ramp)
{
    auto ramp_coordainates{ramp->get_worker_coords()};
    auto half_way{(ramp_coordainates.second + coordinates_.second) / 2};

    while (coordinates_.second > half_way && !stop_thread_.load())
    {
        new_position(std::make_pair(0, -1));
        repaint_worker();
        std::this_thread::sleep_for(speed_);
    }

    while (coordinates_.first != ramp_coordainates.first && !stop_thread_.load())
    {
        if (coordinates_.first - ramp_coordainates.first < 0)
            new_position(std::make_pair(1, 0));
        else
            new_position(std::make_pair(-1, 0));
        repaint_worker();
        std::this_thread::sleep_for(speed_);
    }

    while (coordinates_.second > ramp_coordainates.second && !stop_thread_.load())
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

    while (coordinates_.second < half_way && !stop_thread_.load())
    {
        new_position(std::make_pair(0, 1));
        repaint_worker();
        std::this_thread::sleep_for(speed_);
    }

    while (coordinates_.first != starting_poit_.first && !stop_thread_.load())
    {
        if (coordinates_.first - starting_poit_.first < 0)
            new_position(std::make_pair(1, 0));
        else
            new_position(std::make_pair(-1, 0));
        repaint_worker();
        std::this_thread::sleep_for(speed_);
    }

    while (coordinates_.second < starting_poit_.second && !stop_thread_.load())
    {
        new_position(std::make_pair(0, 1));
        repaint_worker();
        std::this_thread::sleep_for(speed_);
    }
}

void Worker::unpack_ship()
{
    std::this_thread::sleep_for(random_number() * std::chrono::milliseconds(200));
}

int Worker::random_number()
{
    std::uniform_int_distribution<int> dist(1, 8);

    return dist(mt_);
}

Worker::~Worker()
{
    worker_thread_.join();
}