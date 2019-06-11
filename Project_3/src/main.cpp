#include "../include/Setup_scene.hpp"

int main(int argc, char *argv[])
{
    initscr();
    start_color();
    cbreak();
    curs_set(FALSE);
    nodelay(stdscr, 0);
    noecho();
    //raw();

    Setup_scene scene(0.7, 0.3);
    scene.launch_seaport();

    return 0;
}