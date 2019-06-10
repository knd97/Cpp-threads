#pragma once

#include <iostream>
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

    const short TOP_RAMP = 5;
    const short MIDDLE_RAMP = 6;
    const short DOWN_RAMP = 7;
    const short SHIPS_IN_QUEUE = 8;
    const short FREE_WORKERS = 9;

public:
    Window();
    Window(const Window &) = delete;
    Window &operator=(const Window &) = delete;
    Window(Window &&) = default;
    Window &operator=(Window &&) = default;
    ~Window();

    int get_width() const;
    int get_height() const;
    void draw_scene();
    void update_status(const short stat, std::string status, const short color);

    void move_ship(std::pair<int, int> &previous_position, std::pair<int, int> &next_position, bool direction);
    void erase_ship(std::pair<int, int> &previous_position, bool direction);

    void move_worker(std::pair<int, int> &previous_position, std::pair<int, int> &next_position);
    void erase_worker(std::pair<int, int> &previous_position);

private:
    void ncurses_rectangle(int y1, int x1, int y2, int x2);
    void draw_ramps();
    void init_pairs();
    void draw_title();
    void draw_stats();
};