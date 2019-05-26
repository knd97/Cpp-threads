#ifndef Setup_scene_H_
#define Setup_scene_H_

#include <iostream>
#include <thread>
#include <chrono>
#include <random>
#include "Ball.hpp"
#include "Screen.hpp"

class Setup_scene
{
private:
    std::shared_ptr<Screen> screen_;
    std::thread screen_thread_;
    std::vector<std::unique_ptr<Ball>> balls_on_screen_;
    std::atomic<bool> exit_;
    static std::random_device rd_;
    static std::mt19937 mt_;

public:
    Setup_scene();
    Setup_scene(const Setup_scene &) = delete;
    Setup_scene &operator=(const Setup_scene &) = delete;
    Setup_scene(Setup_scene &&) = delete;
    Setup_scene &operator=(Setup_scene &&) = delete;
    ~Setup_scene();

    void launch_balls();

private:
    void check_if_quit();
    void wait(std::chrono::milliseconds period);
    int random_window_index();
};
#endif //Setup_scene_H_