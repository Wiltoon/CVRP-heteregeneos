#pragma once

class Packet{
public:
    int id;
    int loc_x;
    int loc_y;
    int charge;
    Packet(int id, int loc_x, int loc_y, int charge);
};