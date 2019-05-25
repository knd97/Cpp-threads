#ifndef Window_H_
#define Window_H_

#include <iostream>
#include <mutex>
#include <ncurses.h>

class Window
{
private:
    int width_;
    int height_;
    WINDOW *window_;
    static std::mutex mtx_;

public:
    Window() = default;
    Window(int width, int height, int index);
    ~Window();

    void repaint(std::pair<int, int> previous_position, std::pair<int, int> next_position);
    void erase_ball(std::pair<int, int> position);
    int get_maxx() const;
    int get_maxy() const;

private:
    int get_center_x(); //center placement of the window
    int get_center_y();
    void create_window(int index);
    void draw_window(int index);
};
#endif //Window_H_