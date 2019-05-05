#ifndef Screen_H_
#define Screen_H_

#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <array>
#include <memory>
#include "Ball.hpp"

class Screen
{
private:
    int width_;
    int height_;
    std::array<WINDOW *, 3> main_window_;
    std::array<uint8_t, 3> balls_amount_{0, 0, 0};
    std::vector<std::unique_ptr<Ball>> balls_on_screen_;
    std::atomic<bool> exit_;
    std::thread screen_thread_;
    std::thread change_screen_;
    const uint8_t max_balls_ = 5;

public:
    Screen() = default;
    Screen(int width, int height);
    ~Screen();
    void launch_balls();

private:
    int get_center_x(); //center placement of the window
    int get_center_y();
    void check_if_quit();
    void wait(std::chrono::milliseconds period);
    void calc_window_size();
    void draw_window();
    uint8_t rand_window();
    void check_if_change();
    void change_window(WINDOW *old_win, std::pair<WINDOW *, uint8_t> new_win, uint8_t old_win_num);
    void check_ball_status();
    std::pair<WINDOW *, uint8_t> get_new_win();
    void decrease_balls_amount(const std::unique_ptr<Ball> &ball);
};
#endif // Screen_H_