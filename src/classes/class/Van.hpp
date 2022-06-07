#ifndef VAN_H_INCLUDED
#define VAN_H_INCLUDED

#pragma once
#include "Vehicle.hpp"

class Van: public Vehicle{
    Van(int id, Packet deposit);
};

#endif // VAN_H_INCLUDED