#ifndef Screen_H_
#define Screen_H_

#include <iostream>
#include <array>
#include <memory>
#include "Window.hpp"

class Screen
{
private:
    std::array<std::shared_ptr<Window>, 3> main_window_;
    std::array<uint8_t, 3> balls_amount_;
    const uint8_t max_balls_ = 1;
    static std::mutex mtx_;

public:
    Screen();
    Screen(const Screen &scr) = delete;
    ~Screen();
    void decrement_balls(const uint8_t index);
    void increment_balls(const uint8_t index);
    std::shared_ptr<Window> &get_window(uint8_t index);

    uint8_t get_array_size();
    uint8_t get_max_balls() const;
    uint8_t get_amount(const uint8_t index);
};
#endif // Screen_H_