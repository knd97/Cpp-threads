#pragma once

#include <thread>
#include "SeaPort.hpp"
#include "Window.hpp"

class Worker
{
private:
    static const std::pair<int, int> starting_poit_;
    std::thread worker_thread_;
    std::shared_ptr<SeaPort> ramps_;
    std::shared_ptr<Window> window_;
    std::pair<int, int> coordinates_;
    std::pair<int, int> previous_coordinates_;
    std::chrono::milliseconds speed_;
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
    void th_func();
    void worker_to_ramp(std::shared_ptr<Ramp> ramp);
    void back_to_queue();
    void repaint_worker();
    void new_position(std::pair<int, int> step);
};