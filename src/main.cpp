#include "Screen.hpp"

int main()
{
    Screen scr(75, 25);
    scr.launch_balls();
    getch();
    return 0;
}