#ifndef Ball_h_
#define Ball_h_

#include <iostream>
#include <utility>
#include <thread>

class Ball
{
    private:
        std::pair<int, int>coordinates_;        //(x, y) position of ball
        std::thread ball_thread_;

    public:
        Ball() = default;

        ~Ball() = default;
};
#endif // Ball_h_