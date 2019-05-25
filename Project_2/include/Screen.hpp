#ifndef Screen_H_
#define Screen_H_

#include <iostream>
#include <array>
#include <memory>
#include "Window.hpp"

class Screen
{
private:
    std::array<Window, 3> main_window_;
    std::array<int, 3> balls_amount_;
    const int max_balls_ = 1;
    static std::mutex mtx_;

public:
    Screen();
    ~Screen();
    void decrement_balls(const int index);
    void increment_balls(const int index);
    Window &get_window(int index);

    int get_main_window_size() const;
    int get_max_balls() const;
    int get_amount(const int index);
};
#endif // Screen_H_