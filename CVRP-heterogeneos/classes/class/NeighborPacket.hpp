#pragma once

#include "Packet.hpp"
#include <cmath>

struct NeighborPacket{
public:
    Packet origin;
    Packet destination;
    double distance;
    NeighborPacket(Packet& origin, Packet& destination) : 
        origin(origin), destination(destination)
    {
        distance = calculate_distance(origin, destination);
    }
    double calculate_distance(Packet& origin, Packet& destination){
        double dx = (double)destination.loc_x - (double)origin.loc_x;
        double dy = (double)destination.loc_y - (double)origin.loc_y;
        return sqrt(dx * dx + dy * dy);
    }
    Packet getOrigin(){
        return origin;
    }
    Packet getDestination(){
        return destination;
    }
};