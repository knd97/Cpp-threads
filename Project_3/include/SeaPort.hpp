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
    int get_free_ramp() const;
    std::shared_ptr<Ramp> &get_ramp(const int index);
    ~SeaPort() = default;
};