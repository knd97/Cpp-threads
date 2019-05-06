#include "Screen.hpp"

std::mutex m_screen_;

Screen::Screen(int width, int height) : width_{width}, height_{height}, exit_{false}
{
    initscr();
    cbreak();
    curs_set(FALSE);
    draw_window();
}

void Screen::calc_window_size()
{
    for (size_t i = 0; i < main_window_.size(); ++i)
    {
        main_window_[i] = newwin(height_, static_cast<int>(width_ / 3), get_center_y(),
                                 get_center_x() + (i * static_cast<int>(width_ / 3) - 1));
    }
}

void Screen::draw_window()
{
    calc_window_size();
    refresh();
    for (size_t i = 0; i < main_window_.size(); ++i)
    {
        box(main_window_[i], 0, 0);
        wrefresh(main_window_[i]);
    }
}

void Screen::launch_balls()
{
    screen_thread_ = std::thread([&]() { check_if_quit(); });
    change_screen_ = std::thread([&]() { check_if_change(); });

    while (!exit_.load())
    {
        check_ball_status();
        auto win_number{rand_window()};
        if (balls_amount_[win_number] < max_balls_)
        {
            balls_on_screen_.push_back(std::make_unique<Ball>(main_window_[win_number]));
            balls_on_screen_.back()->th_start();
            balls_amount_[win_number]++;
            wait(std::chrono::milliseconds(3000));
        }
    }
}

void Screen::check_ball_status()
{
    for (auto &it : balls_on_screen_)
    {
        if (it->check_status() && !it->check_if_ended())
        {
            decrease_balls_amount(it);
        }
    }
}

void Screen::decrease_balls_amount(const std::unique_ptr<Ball> &ball)
{
    auto position = std::find(main_window_.begin(), main_window_.end(), ball->get_window()) - main_window_.begin();
    balls_amount_[position]--;
    ball->set_ended();
}

void Screen::check_if_change()
{
    while (!exit_.load())
    {
        auto [new_win, win_num] = get_new_win();
        auto old_win_num{rand_window()};

        while (old_win_num == win_num)
        {
            old_win_num = rand_window();
        }

        auto old_win{main_window_[old_win_num]};
        change_window(old_win, std::make_pair(new_win, win_num), old_win_num);
        wait(std::chrono::milliseconds(700));
    }
}

void Screen::change_window(WINDOW *old_win, std::pair<WINDOW *, uint8_t> new_win, uint8_t old_win_num)
{
    std::lock_guard<std::mutex> lg(m_screen_);
    auto ball_to_move = std::find_if(balls_on_screen_.begin(), balls_on_screen_.end(),
                                     [&old_win](const std::unique_ptr<Ball> &ball) { return ball->get_window() == old_win; });
    if (ball_to_move != balls_on_screen_.end())
    {
        (*ball_to_move)->set_new_window(new_win.first);
        balls_amount_[old_win_num]--;
        balls_amount_[new_win.second]++;
    }
}

uint8_t Screen::rand_window()
{
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<uint8_t> dist(0, main_window_.size() - 1);

    return dist(mt);
}

std::pair<WINDOW *, uint8_t> Screen::get_new_win()
{
    auto position{rand_window()};
    while (balls_amount_[position] == max_balls_)
    {
        position = rand_window();
    }
    return std::make_pair(main_window_[position], position);
}

void Screen::wait(std::chrono::milliseconds period)
{
    auto start{std::chrono::system_clock::now()};
    std::chrono::system_clock::time_point end{};

    while (std::chrono::duration_cast<std::chrono::milliseconds>(end - start) < period && !exit_.load())
    {
        end = std::chrono::system_clock::now();
    }
}

void Screen::check_if_quit()
{
    while (!exit_.load())
    {
        if (static_cast<int>(getch()) == 27)
            exit_.store(true);
    }
}

int Screen::get_center_x()
{
    return static_cast<int>((COLS - width_) / 2);
}

int Screen::get_center_y()
{
    return static_cast<int>((LINES - height_) / 2);
}

Screen::~Screen()
{
    change_screen_.join();
    screen_thread_.join();

    for (auto &ball : balls_on_screen_)
        ball->th_stop();

    balls_on_screen_.clear();
    endwin();
}