#include "Packet.hpp"
#include <iostream>

Packet::Packet(int id,int x, int y, int cap):
    id(id),
    loc_x(x),
    loc_y(y),
    charge(cap),
{}