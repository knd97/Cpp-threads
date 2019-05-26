#ifndef Window_H_
#define Window_H_

#include <iostream>
#include <mutex>
#include <memory>
#include <ncurses.h>

class Window
{
private:
    int width_;
    int height_;
    std::unique_ptr<WINDOW, void (*)(WINDOW *)> window_;
    static std::mutex mtx_;

public:
    Window(int width, int height, int index);
    Window() = delete;
    Window(const Window &) = delete;
    Window &operator=(const Window &) = delete;
    Window(Window &&) = default;
    Window &operator=(Window &&) = default;
    ~Window();

    void repaint_ball(std::pair<int, int> previous_position, std::pair<int, int> next_position);
    void erase_ball(std::pair<int, int> position);
    int get_maxx() const;
    int get_maxy() const;

private:
    int get_center_x(); //center placement of the window
    int get_center_y();
};
#endif //Window_H_