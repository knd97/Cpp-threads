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
        std::pair<int, int>coordinates_;        //(x, y) position of ball
        std::thread ball_thread_;
        std::chrono::milliseconds interval_;
        const char ball_symbol_ = 'o';
        bool stop_thread_;

    public:
<<<<<<< HEAD
        Ball(WINDOW *window, std::chrono::milliseconds period);
=======
        Ball(std::pair<int, int>position, std::chrono::milliseconds period);
        
>>>>>>> init
        ~Ball();
};
#endif // Ball_h_