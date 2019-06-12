#include "../include/Setup_scene.hpp"

std::condition_variable cv_workers_;
std::condition_variable cv_ships_;

int main(int argc, char *argv[])
{
    initscr();
    start_color();
    cbreak();
    curs_set(FALSE);
    nodelay(stdscr, 0);
    noecho();

    Setup_scene scene(0.7, 0.3);
    scene.launch_seaport();

    return 0;
}