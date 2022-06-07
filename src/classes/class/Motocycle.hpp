#ifndef MOTOCYCLE_H_INCLUDED
#define MOTOCYCLE_H_INCLUDED

#pragma once
#include "Vehicle.hpp"

class Motocycle: public Vehicle{
    Motocycle(int id, Packet deposit);
};

#endif // MOTOCYCLE_H_INCLUDED