#pragma once
#include <iostream>
#include <vector>
#include "Packet.hpp"

struct Vehicle
{
    int id = 0;
    int charge_max = 200;
    std::vector<Packet> packets_order;
    bool used = false;

    void addPacket(Packet packet);

    void useVehicle(bool use);
};