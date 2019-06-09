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
    std::pair<int, int> ship_coordinates_;
    std::pair<int, int> worker_coordinates_;

public:
};