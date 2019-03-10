#ifndef Screen_h_
#define Screen_h_
#include <iostream>
#include <ncurses.h>

class Screen
{
    private:
        int startx_;
        int starty_;
        int width_;
        int height_;
        WINDOW *main_window;

    public:
        Screen() = default;
        Screen(int startX, int startY, int width, int height);


        ~Screen();
};
#endif // Screen_h_