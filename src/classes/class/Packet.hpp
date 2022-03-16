#ifndef PACKET_H_INCLUDED
#define PACKET_H_INCLUDED

#pragma once

#include <iostream>
#include <vector>
#include <string>

#include "../../../src/Point.hpp"

class Packet{
public:
    int id;
    std::string id_s;
    Point pt;
    double loc_x;
    double loc_y;
    int charge;
    double price = 0.0;
    std::vector<int> neighbors;
    Packet();
    Packet(int id, double loc_x, double loc_y, int charge);
    Packet(int id, std::string id_s, double loc_x, double loc_y, int charge);
};

#endif // PACKET_H_INCLUDED