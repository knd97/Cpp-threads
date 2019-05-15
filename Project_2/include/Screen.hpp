#ifndef Screen_H_
#define Screen_H_

#include <iostream>
#include <array>
#include <memory>
#include "Window.hpp"

class Screen
{
private:
    std::array<std::unique_ptr<Window>, 3> main_window_;
    std::array<uint8_t, 3> balls_amount_{0, 0, 0};
    const static uint8_t max_balls_ = 5;
    mutable std::mutex mtx_;

public:
    Screen();
    ~Screen() = default;
    void decrement_balls(const uint8_t index);
    void increment_balls(const uint8_t index);
    std::unique_ptr<Window> &get_window(uint8_t index);

    uint8_t get_array_size();
    uint8_t get_max_balls() const;
    uint8_t get_amount(const uint8_t index) const;
};
#endif // Screen_H_