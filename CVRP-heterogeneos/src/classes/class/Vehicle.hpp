#ifndef VEHICLE_H_INCLUDED
#define VEHICLE_H_INCLUDED

#pragma once

#include <iostream>
#include <vector>
#include "Packet.hpp"

class Vehicle
{
public:
    int id = 0;
    int charge_max = 200;
    int current_charge = 0;
    int cust = 20;
    Packet origin;
    std::vector<Packet> deliveries;
    bool used = false;

    Vehicle(int id, int charge, int cust, Packet deposit);
    void addPacket(Packet packet);
    void useVehicle(bool use);
};

#endif // VEHICLE_H_INCLUDED