#include "../include/Ship.hpp"

const std::pair<int, int> Ship::starting_point_ = {15, 12};
const std::chrono::milliseconds Ship::speed_ = std::chrono::milliseconds(100);
std::mutex Ship::m_ship_;

Ship::Ship(std::pair<int, int> stop_coordinates, std::shared_ptr<Window> win, std::shared_ptr<SeaPort> seaport) : coordinates_wait_{stop_coordinates},
                                                                                                                  coordinates_{starting_point_},
                                                                                                                  stop_thread_{false},
                                                                                                                  window_{std::move(win)},
                                                                                                                  seaport_{std::move(seaport)},
                                                                                                                  port_index_{-1}
{
}

void Ship::start()
{
    ship_thread_ = std::thread([&]() { th_func(); });
}

void Ship::th_func()
{
    //while (!stop_thread_.load())
    {
        ship_to_queue();
        std::unique_lock lck(m_ship_);
        c_v_.wait(lck, [&]() { return seaport_->free_ramp(); });

        int free_ramp = seaport_->get_free_ramp();
        seaport_->occupate_ramp(free_ramp);
        ship_to_ramp(seaport_->get_ramp(free_ramp));
        seaport_->ship_parked(free_ramp);
        //launch_ship();
    }
}

void Ship::stop()
{
    stop_thread_.store(true);
    window_->erase_ship(coordinates_, direction_);
    c_v_.notify_all();
}

void Ship::new_position(int step, bool direction)
{
    previous_coordinates_ = coordinates_;
    previous_direction_.store(direction_);
    direction_ = direction;

    if (direction)
    {
        coordinates_.second += step;
    }
    else
    {
        coordinates_.first += step;
    }
}

void Ship::ship_to_ramp(std::shared_ptr<Ramp> ramp)
{
    auto ramp_coordinates{ramp->get_ship_coords()};
    auto half_way{(coordinates_.second + ramp_coordinates.second) / 2};

    while (coordinates_.second < half_way)
    {
        new_position(1, true);
        repaint_ship();
        std::this_thread::sleep_for(speed_);
    }

    while (coordinates_.first != ramp_coordinates.first)
    {
        if (coordinates_.first - ramp_coordinates.first < 0)
            new_position(1, false);
        else
            new_position(-1, false);
        repaint_ship();
        std::this_thread::sleep_for(speed_);
    }

    while (coordinates_.second < ramp_coordinates.second)
    {
        new_position(1, true);
        repaint_ship();
        std::this_thread::sleep_for(speed_);
    }
}

void Ship::ship_to_queue()
{
    while (coordinates_.first < coordinates_wait_.first)
    {
        new_position(1, false);
        repaint_ship();
        std::this_thread::sleep_for(speed_);
    }

    while (coordinates_.second < coordinates_wait_.second)
    {
        new_position(1, true);
        repaint_ship();
        std::this_thread::sleep_for(speed_);
    }
}

void Ship::launch_ship()
{
    auto current_x{coordinates_.second};

    while (coordinates_.second > current_x - 13)
    {
        new_position(-1, true);
        repaint_ship();
        std::this_thread::sleep_for(speed_);
    }

    while (coordinates_.first > 10)
    {
        new_position(-1, false);
        repaint_ship();
        std::this_thread::sleep_for(speed_);
    }
}

void Ship::repaint_ship()
{
    window_->erase_ship(previous_coordinates_, previous_direction_);
    window_->move_ship(previous_coordinates_, coordinates_, direction_);
}

Ship::~Ship()
{
    ship_thread_.join();
}