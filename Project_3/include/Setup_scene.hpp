#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <atomic>
#include <thread>
#include <chrono>
#include "Worker.hpp"
#include "Ship.hpp"
#include "Window.hpp"
#include "SeaPort.hpp"

class Setup_scene
{
private:
    std::thread screen_thread_;
    std::vector<std::unique_ptr<Ship>> ships_;
    std::vector<std::unique_ptr<Worker>> workers_;
    std::shared_ptr<Window> main_winodw_;
    std::shared_ptr<SeaPort> ramps_;
    std::atomic<bool> exit_;
    double new_ship_freq_;
    double weather_;
    const std::chrono::milliseconds period_;

public:
    Setup_scene(double frequency, double weather_);
    Setup_scene(const Setup_scene &) = delete;             //copy constructor
    Setup_scene &operator=(const Setup_scene &) = delete;  //copy assignment
    Setup_scene(const Setup_scene &&) = delete;            //move constructor
    Setup_scene &operator=(const Setup_scene &&) = delete; //move assignment
    ~Setup_scene();

    void launch_seaport();

private:
    void check_if_quit();
    void wait();
    void launch_workers();
};