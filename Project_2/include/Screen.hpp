#ifndef Screen_H_
#define Screen_H_

#include <iostream>
#include <array>
#include <vector>
#include <random>
#include <algorithm>
#include "Window.hpp"

class Screen
{
private:
    constexpr static int win_number_ = 3;
    const int max_balls_ = 1;
    static std::mutex mtx_;
    static std::random_device rd_;
    static std::mt19937 mt_;
    std::vector<Window> main_window_;
    std::array<int, win_number_> balls_amount_;

public:
    Screen();
    Screen(const Screen &) = delete;
    Screen &operator=(const Screen &) = delete;
    Screen(Screen &&) = delete;
    Screen &operator=(Screen &&) = delete;
    ~Screen();

    void decrement_balls_amount(const int window_index);
    void increment_balls_amount(const int window_index);
    Window &get_window(int window_index);

    int get_free_window();
    int get_main_window_size() const;
    int get_max_balls() const;
    int get_balls_amount(const int window_index) const;
};
#endif // Screen_H_