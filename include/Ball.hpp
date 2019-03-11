#ifndef Ball_h_
#define Ball_h_

#include <iostream>
#include <utility>
#include <thread>
#include <chrono>
#include <ncurses.h>

class Ball
{
    private:
        std::pair<int, int>coordinates_;                //(x, y) position of ball
        std::thread ball_thread_;
        bool stop_thread_;
        static std::chrono::milliseconds interval_;
        static const char * ball_symbol_;

    public:
        Ball();
        Ball(WINDOW *window);
        ~Ball();
        
        void th_start();
        
    private:
        void th_func();
        void move();
};
#endif // Ball_h_