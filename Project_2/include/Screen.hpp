#ifndef Screen_H_
#define Screen_H_

#include <iostream>
#include <vector>
#include <array>
#include <memory>
#include "Ball.hpp"

class Screen
{
    private:
        int width_;
        int height_;
        std::array<WINDOW *, 3>main_window_;
        std::vector<std::unique_ptr<Ball>> balls_on_screen_;
        std::atomic<bool> exit_;
        std::thread screen_thread_;

    public:
        Screen() = default;
        Screen(int width, int height);
        ~Screen();
        void launch_balls();
    private:
        int get_center_x();                                        //center placement of the window
        int get_center_y();
        void check_if_quit();
        void wait(std::chrono::milliseconds period);
        void calc_window_size();
        void draw_window();
        unsigned rand_window();
};
#endif // Screen_H_