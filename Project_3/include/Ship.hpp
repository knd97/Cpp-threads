#pragma once

#include <utility>
#include <atomic>
#include <thread>
#include "Window.hpp"
#include "SeaPort.hpp"

class Ship
{
private:
    static const std::pair<int, int> starting_point_;
    std::pair<int, int> coordinates_wait_;
    std::pair<int, int> coordinates_;
    std::pair<int, int> previous_coordinates_;
    std::atomic<bool> direction_;
    std::atomic<bool> previous_direction_;
    std::atomic<bool> stop_thread_;
    std::thread ship_thread_;
    std::shared_ptr<Window> window_;
    std::shared_ptr<SeaPort> seaport_;
    static const std::chrono::milliseconds speed_;

public:
    Ship(std::pair<int, int> stop_coordinates, std::shared_ptr<Window> win, std::shared_ptr<SeaPort> seaport);
    Ship(const Ship &) = delete;
    Ship &operator=(const Ship &) = delete;
    Ship(Ship &&) = delete;
    Ship &operator=(Ship &&) = delete;
    ~Ship();

    void stop();
    void start();

private:
    void ship_to_ramp();
    void ship_to_queue();
    void th_func();
    void new_position(int step, bool direction);
};