#ifndef Screen_h_
#define Screen_h_
#include <iostream>
#include <ncurses.h>

class Screen
{
    private:
        int width_;
        int height_;
        WINDOW *main_window_;

    public:
        Screen() = default;
<<<<<<< HEAD
        Screen(int width, int height);
=======
        Screen(int startX, int startY, int width, int height);
>>>>>>> init

        ~Screen();
    private:
        int get_center_x();             //center placement of the window
        int get_center_y();
};
#endif // Screen_h_