#pragma once

#include <atomic>
#include <utility>
#include <mutex>
#include <condition_variable>

class Ramp
{
private:
    std::mutex mtx_;
    std::atomic<bool> is_free_;
    std::atomic<bool> is_worker_;
    std::atomic<bool> is_ship_coming_;
    std::pair<int, int> ship_coordinates_;
    std::pair<int, int> worker_coordinates_;

public:
    Ramp() = delete;
    Ramp(std::pair<int, int> coordinates);
    Ramp(const Ramp &) = delete;
    Ramp &operator=(const Ramp &) = delete;
    Ramp(Ramp &&) = delete;
    Ramp &operator=(Ramp &&) = delete;
    ~Ramp() = default;

    bool check_if_free();
    bool check_if_worker();
    bool check_if_ship_coming();

    std::pair<int, int> get_ship_coords();
    std::pair<int, int> get_worker_coords();
};