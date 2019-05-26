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

    static std::random_device rd_;
    static std::mt19937 mt_;
    static std::mutex m_ball_;
    static std::condition_variable c_v_;
    static std::atomic<bool> possible_move_;

    static const std::chrono::milliseconds interval_;
    static const std::chrono::seconds lifetime_;
    static const std::vector<std::pair<int, int>> possible_directory_;

public:
    Ball(int index, std::shared_ptr<Screen> scr);
    Ball() = delete;
    Ball(const Ball &) = delete;
    Ball &operator=(const Ball &) = delete;
    Ball(Ball &&) = delete;
    Ball &operator=(Ball &&) = delete;
    ~Ball();

    void th_start();
    void th_stop();

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