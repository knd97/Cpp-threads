#ifndef Ball_h_
#define Ball_h_

#include <iostream>
#include <utility>
#include <thread>
#include <chrono>
#include <random>
#include <mutex>
#include <ncurses.h>

class Ball
{
    private:
        WINDOW *window_;
        std::pair<int, int>coordinates_;                                            //(x, y) position of ball
        std::thread ball_thread_;
        bool stop_thread_;
        static std::chrono::milliseconds interval_;
        static const char *ball_symbol_;
        std::vector<std::pair<int, int>> possible_moves_{ {1, 0}, {-1, 0}, {0, 1}, 
                                                        {0, -1}, {1, 1}, {-1, 1}, 
                                                        {-1, -1}, {1, -1} };

    public:
        Ball(WINDOW *window);
        ~Ball();
        
        void th_start();

    private:
        void th_func();
        void move();
        std::pair<int, int> random_direction() const;
        void repaint_ball(std::pair<int, int> previous_position);
        void new_coordinates(std::pair<int, int> direction);
};
#endif // Ball_h_