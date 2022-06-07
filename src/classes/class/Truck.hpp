#ifndef TRUCK_H_INCLUDED
#define TRUCK_H_INCLUDED

#pragma once
#include "Vehicle.hpp"

class Truck: public Vehicle{
    Truck(int id, Packet deposit);
};

#endif // TRUCK_H_INCLUDED