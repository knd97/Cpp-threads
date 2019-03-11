#ifndef Ball_h_
#define Ball_h_

#include <iostream>
#include <utility>
#include <thread>
#include <chrono>

class Ball
{
    private:
        std::pair<int, int>coordinates_;        //(x, y) position of ball
        std::thread ball_thread_;
        std::chrono::milliseconds interval_;
        const char ball_symbol_ = 'o';

    public:
        Ball(std::pair<int, int>position, std::chrono::milliseconds period);
        
        ~Ball();
};
#endif // Ball_h_