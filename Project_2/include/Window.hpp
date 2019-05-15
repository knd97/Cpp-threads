#ifndef Window_H_
#define Window_H_

#include <iostream>
#include <mutex>
#include <ncurses.h>

class Window
{
private:
    uint8_t width_;
    uint8_t height_;
    WINDOW *window_;
    static std::mutex mtx_;

public:
    Window() = default;
    Window(uint8_t width, uint8_t height, uint8_t index);
    ~Window();

    void repaint(std::pair<int, int> previous_position, std::pair<int, int> next_position);
    void erase_ball(std::pair<int, int> position);
    WINDOW *get_window();

private:
    int get_center_x(); //center placement of the window
    int get_center_y();
    void create_window(uint8_t index);
    void draw_window(uint8_t index);
};
#endif //Window_H_