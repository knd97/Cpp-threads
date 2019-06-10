#include "../include/SeaPort.hpp"

SeaPort::SeaPort(int width, int height)
{
    for (size_t i = 0; i < 3; ++i)
    {
        ramps_.push_back(std::make_shared<Ramp>(std::make_pair(25 + i * 2, 120)));
    }
}

int SeaPort::get_free_ramp() const
{
    for (size_t i = 0; i < ramps_.size(); ++i)
    {
        if (ramps_[i]->check_if_free())
            return i;
    }
    return -1;
}

std::shared_ptr<Ramp> &SeaPort::get_ramp(const int index)
{
    return ramps_.at(index);
}