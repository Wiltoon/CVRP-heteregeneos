#ifndef PACKET_H_INCLUDED
#define PACKET_H_INCLUDED

#pragma once

class Packet{
public:
    int id;
    int loc_x;
    int loc_y;
    int charge;
    double price = 0.0;
    Packet(int id, int loc_x, int loc_y, int charge);
};

#endif // PACKET_H_INCLUDED