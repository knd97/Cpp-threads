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
        if (ramps_[i]->check_if_free() && !ramps_[i]->check_if_ship_coming())
            return i;
    }
    return -1;
}

bool SeaPort::free_ramp()
{
    for (size_t i = 0; i < ramps_.size(); ++i)
    {
        if (ramps_[i]->check_if_free() && !ramps_[i]->check_if_ship_coming())
            return true;
    }
    return false;
}

std::shared_ptr<Ramp> &SeaPort::get_ramp(const int index)
{
    return ramps_.at(index);
}

bool SeaPort::check_if_worker_needed()
{
    for (auto &it : ramps_)
    {
        if (!it->check_if_ship_coming() && !it->check_if_worker() && !it->check_if_free())
        {
            return true;
        }
    }
    return false;
}

int SeaPort::worker_needed()
{
    for (size_t i = 0; i < ramps_.size(); ++i)
    {
        if (!ramps_[i]->check_if_ship_coming() && !ramps_[i]->check_if_worker() && !ramps_[i]->check_if_free())
            return i;
    }
    return -1;
}

void SeaPort::occupate_ramp(const int index)
{
    ramps_[index]->ship_is_coming();
}

void SeaPort::worket_to_ramp(const int index)
{
    ramps_[index]->send_worker();
}

void SeaPort::ship_parked(const int index)
{
    ramps_[index]->moor_ship();
}

void SeaPort::worker_finished(const int index)
{
    ramps_[index]->worker_to_queue();
}

void SeaPort::ship_exit(const int index)
{
    ramps_[index]->leave_ship();
}