#pragma once

#include <vector>
#include "Ramp.hpp"
#include "Window.hpp"

class SeaPort
{
private:
    std::vector<std::shared_ptr<Ramp>> ramps_;

public:
    SeaPort(int width, int height);
    SeaPort(const SeaPort &) = delete;
    SeaPort &operator=(const SeaPort &) = delete;
    SeaPort(SeaPort &&) = delete;
    SeaPort &operator=(SeaPort &&) = delete;
    ~SeaPort() = default;

    int get_free_ramp() const;
    std::shared_ptr<Ramp> &get_ramp(const int index);
    bool free_ramp();
    int worker_needed();
    bool check_if_worker_needed();

    void occupate_ramp(const int index);
    void ship_parked(const int index);
    void worket_to_ramp(const int index);
    void worker_finished(const int index);
    void ship_exit(const int index);
};