#ifndef BALL_H_
#define BALL_H_

#include <iostream>
#include <utility>
#include <thread>
#include <chrono>
#include <random>
#include <atomic>
#include <condition_variable>
#include "Screen.hpp"

class Ball
{
private:
    std::shared_ptr<Screen> screen_;
    int window_index_;
    int next_win_index_;
    std::pair<int, int> coordinates_; //random move from 'possible_moves'
    std::pair<int, int> position_;    //(x, y) position of ball
    std::thread ball_thread_;

    std::atomic<bool> stop_thread_;

    static std::mutex m_ball_;
    static std::condition_variable c_v_;
    std::atomic<bool> possible_move_;

    const std::chrono::milliseconds interval_ = std::chrono::milliseconds(40);
    const std::chrono::seconds lifetime_ = std::chrono::seconds(5);
    const std::vector<std::pair<int, int>> possible_directory_{{1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 1}, {-1, 1}, {-1, -1}, {1, -1}};

public:
    Ball() = delete;
    Ball(int index, std::shared_ptr<Screen> scr);
    ~Ball();

    void th_start();
    void th_stop();
    bool check_stop();
    int get_win_index();

private:
    void transit();
    void set_next_wind();
    void check_free_window();

    void th_func();
    void move();
    std::pair<int, int> random_direction() const;
    void new_position();
    void check_if_rebound();
};
#endif //BALL_H_