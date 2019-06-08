#pragma once

#include <ncurses.h>
#include <memory>
#include <mutex>

class Window
{
private:
    int width_;
    int height_;
    std::unique_ptr<WINDOW, void (*)(WINDOW *)> window_;
    static std::mutex mtx_;

    //ncurses colors
    const int WHITE = 1;
    const int RED = 2;
    const int BLUE = 3;
    const int GREEN = 4;
    const int PINK = 5;
    const int YELLOW = 6;

public:
    Window();
    Window(const Window &) = delete;
    Window &operator=(const Window &) = delete;
    Window(Window &&) = default;
    Window &operator=(Window &&) = default;
    ~Window();

private:
    void draw_scene();
    void draw_ramps();

    void init_pairs();
};