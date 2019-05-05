#ifndef BALL_H_
#define BALL_H_

#include <iostream>
#include <utility>
#include <thread>
#include <chrono>
#include <random>
#include <mutex>
#include <atomic>
#include <ncurses.h>

class Ball
{
private:
    WINDOW *window_;
    std::pair<int, int> coordinates_; //random move from 'possible_moves'
    std::pair<int, int> position_;    //(x, y) position of ball
    std::thread ball_thread_;
    std::atomic<bool> stop_thread_;
    std::atomic<bool> ended_;
    const std::chrono::milliseconds interval_ = std::chrono::milliseconds(65);
    const std::chrono::seconds lifetime_ = std::chrono::seconds(10);
    const char *ball_symbol_ = "o";
    const std::vector<std::pair<int, int>> possible_moves_{{1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 1}, {-1, 1}, {-1, -1}, {1, -1}};

public:
    Ball() = default;
    Ball(WINDOW *window);
    ~Ball();

    void th_start();
    void th_stop();
    void set_new_window(WINDOW *new_win);
    WINDOW *get_window() const;

    bool check_status();
    void set_ended();
    bool check_if_ended();

private:
    void th_func();
    void move();
    std::pair<int, int> random_direction() const;
    void repaint_ball(std::pair<int, int> previous_position);
    void new_position();
    void check_if_rebound();
    void erase_ball();
};
#endif //BALL_H_