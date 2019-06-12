#pragma once

#include <thread>
#include <random>
#include "SeaPort.hpp"
#include "Window.hpp"
#include "Ship.hpp"

class Worker
{
private:
    const std::pair<int, int> starting_poit_;
    std::thread worker_thread_;
    std::shared_ptr<SeaPort> seaport_;
    std::shared_ptr<Window> window_;
    std::pair<int, int> coordinates_;
    std::pair<int, int> previous_coordinates_;
    static const std::chrono::milliseconds speed_;
    static std::mutex m_worker_;
    static std::random_device rd_;
    static std::mt19937 mt_;
    std::atomic<bool> stop_thread_;

public:
    Worker(std::pair<int, int> coordinates, std::shared_ptr<SeaPort> ramps, std::shared_ptr<Window> main_window);
    Worker(const Worker &) = delete;
    Worker &operator=(const Worker &) = delete;
    Worker(Worker &&) = delete;
    Worker &operator=(Worker &&) = delete;
    ~Worker();

    void stop();
    void start();

private:
    void unpack_ship();
    void th_func();
    void worker_to_ramp(std::shared_ptr<Ramp> ramp);
    void back_to_queue();
    void repaint_worker();
    void new_position(std::pair<int, int> step);
    int random_number();
};