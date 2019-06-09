#include "../include/Ramp.hpp"

Ramp::Ramp(std::pair<int, int> coordinates) : is_free_{false},
                                              is_worker_{false},
                                              is_ship_coming_{false},
                                              ship_coordinates_{coordinates},
                                              worker_coordinates_{coordinates}
{
    ship_coordinates_.second -= 2;
    worker_coordinates_.second += 1;
}

bool Ramp::check_if_free()
{
    std::lock_guard l_g_(mtx_);
    return is_free_.load();
}

bool Ramp::check_if_worker()
{
    std::lock_guard l_g_(mtx_);
    return is_worker_.load();
}

bool Ramp::check_if_ship_coming()
{
    std::lock_guard l_g_(mtx_);
    return is_ship_coming_.load();
}

std::pair<int, int> Ramp::get_ship_coords()
{
    std::lock_guard l_g_(mtx_);
    return ship_coordinates_;
}

std::pair<int, int> Ramp::get_worker_coords()
{
    std::lock_guard l_g_(mtx_);
    return worker_coordinates_;
}