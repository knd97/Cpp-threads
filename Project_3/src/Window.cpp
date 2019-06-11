#include "../include/Window.hpp"

std::mutex Window::mtx_;

Window::Window() : width_{getmaxx(stdscr)},
                   height_{getmaxy(stdscr)},
                   window_(newwin(getmaxy(stdscr), getmaxx(stdscr), 0, 0),
                           [](WINDOW *w) {
                               delwin(w);
                               endwin();
                           })
{
    use_default_colors();
    refresh();
    box(window_.get(), 0, 0);
    wrefresh(window_.get());
}

void Window::draw_scene()
{
    init_pairs();
    draw_title();
    draw_stats();
    draw_ramps();
}

void Window::draw_ramps()
{
    wattron(window_.get(), COLOR_PAIR(GREEN));
    for (size_t i = 0; i < 3; ++i)
    {
        for (size_t j = 0; j < 7; ++j)
        {
            mvwprintw(window_.get(), height_ / 2 - 5 + i * 2, width_ / 2 + j, "-");
        }
        mvwprintw(window_.get(), height_ / 2 - 5 + i * 2 + 1, width_ / 2 + 3, "|");
    }
    for (size_t j = 0; j < 7; ++j)
    {
        mvwprintw(window_.get(), height_ / 2 - 5 + 3 * 2, width_ / 2 + j, "-");
    }
    wattroff(window_.get(), COLOR_PAIR(GREEN));
    wrefresh(window_.get());
}

void Window::init_pairs()
{
    init_color(COLOR_GREEN, 0, 700, 0);
    init_color(COLOR_RED, 700, 0, 0);

    init_pair(WHITE, COLOR_WHITE, -1);
    init_pair(RED, COLOR_RED, -1);
    init_pair(BLUE, COLOR_BLUE, -1);
    init_pair(GREEN, COLOR_GREEN, -1);
    init_pair(PINK, COLOR_MAGENTA, -1);
    init_pair(YELLOW, COLOR_YELLOW, -1);
}

void Window::ncurses_rectangle(int y1, int x1, int y2, int x2)
{
    mvwhline(window_.get(), y1, x1, 0, x2 - x1);
    mvwhline(window_.get(), y2, x1, 0, x2 - x1);
    mvwvline(window_.get(), y1, x1, 0, y2 - y1);
    mvwvline(window_.get(), y1, x2, 0, y2 - y1);
    mvwaddch(window_.get(), y1, x1, ACS_ULCORNER);
    mvwaddch(window_.get(), y2, x1, ACS_LLCORNER);
    mvwaddch(window_.get(), y1, x2, ACS_URCORNER);
    mvwaddch(window_.get(), y2, x2, ACS_LRCORNER);
}

void Window::draw_title()
{
    wattron(window_.get(), COLOR_PAIR(PINK));
    ncurses_rectangle(2, 2, 4, width_ - 3);
    mvwprintw(window_.get(), 3, (width_ / 2) - 8, "Seaport Simulator");
    wattroff(window_.get(), COLOR_PAIR(PINK));
    wrefresh(window_.get());
}

void Window::draw_stats()
{
    wattron(window_.get(), COLOR_PAIR(WHITE));
    mvwprintw(window_.get(), TOP_RAMP, 4, "Top  ramp: ");
    mvwprintw(window_.get(), MIDDLE_RAMP, 4, "Middle ramp: ");
    mvwprintw(window_.get(), DOWN_RAMP, 4, "Down ramp: ");
    mvwprintw(window_.get(), SHIPS_IN_QUEUE, 4, "Ships in queue: ");
    mvwprintw(window_.get(), FREE_WORKERS, 4, "Free workers: ");
    wattroff(window_.get(), COLOR_PAIR(WHITE));
    update_status(TOP_RAMP, "Free", GREEN);
    update_status(MIDDLE_RAMP, "Free", GREEN);
    update_status(DOWN_RAMP, "Free", GREEN);
    update_status(SHIPS_IN_QUEUE, "0", GREEN);
    update_status(FREE_WORKERS, "5", GREEN);
}

void Window::move_ship(std::pair<int, int> &previous_position, std::pair<int, int> &next_position, bool direction)
{
    std::lock_guard l_g_(mtx_);
    if (direction)
    {
        mvwprintw(window_.get(), (next_position.first), next_position.second - 1, "#");
        mvwprintw(window_.get(), (next_position.first), next_position.second, "#");
        mvwprintw(window_.get(), (next_position.first), next_position.second + 1, "#");
    }
    else
    {
        mvwprintw(window_.get(), (next_position.first) - 1, next_position.second, "#");
        mvwprintw(window_.get(), (next_position.first), next_position.second, "#");
        mvwprintw(window_.get(), (next_position.first) + 1, next_position.second, "#");
    }
    wrefresh(window_.get());
}

void Window::erase_ship(std::pair<int, int> &previous_position, bool direction)
{
    std::lock_guard l_g_(mtx_);
    if (direction)
    {
        mvwprintw(window_.get(), (previous_position.first), previous_position.second - 1, " ");
        mvwprintw(window_.get(), (previous_position.first), previous_position.second, " ");
        mvwprintw(window_.get(), (previous_position.first), previous_position.second + 1, " ");
    }
    else
    {
        mvwprintw(window_.get(), (previous_position.first) - 1, previous_position.second, " ");
        mvwprintw(window_.get(), (previous_position.first), previous_position.second, " ");
        mvwprintw(window_.get(), (previous_position.first) + 1, previous_position.second, " ");
    }
    wrefresh(window_.get());
}

void Window::update_status(const short stat, std::string status, const short color)
{
    std::lock_guard l_g_(mtx_);
    wattron(window_.get(), COLOR_PAIR(color));
    mvwprintw(window_.get(), stat, 20, status.c_str());
    wattroff(window_.get(), COLOR_PAIR(color));
    wrefresh(window_.get());
}

void Window::move_worker(std::pair<int, int> &previous_position, std::pair<int, int> &next_position)
{
    std::lock_guard l_g_(mtx_);
    mvwprintw(window_.get(), previous_position.first, previous_position.second, " ");
    mvwprintw(window_.get(), next_position.first, next_position.second, "o");
    wrefresh(window_.get());
}

void Window::erase_worker(std::pair<int, int> &previous_position)
{
    std::lock_guard l_g_(mtx_);
    mvwprintw(window_.get(), previous_position.first, previous_position.second, " ");
    wrefresh(window_.get());
}

int Window::get_height() const
{
    return height_;
}

int Window::get_width() const
{
    return width_;
}

Window::~Window()
{
    endwin();
}