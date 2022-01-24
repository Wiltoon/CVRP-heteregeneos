#ifndef PACKETCENTROID_H_INCLUDED
#define PACKETCENTROID_H_INCLUDED

#pragma once

#include <iostream>
#include <vector>

#include "Packet.hpp"
#include "Vehicle.hpp"

class PacketCentroid{
private:
    int id_cluster;
    std::vector<Packet> packets_local;
    int charge_total = 0;
public:
    std::vector<double> center;
    std::vector<Vehicle> vehicles;

    PacketCentroid(
        int id_cluster, 
        std::vector<double> center_, 
        std::vector<Packet> packets_local 
    );
    double getCenterX();
    double getCenterY();

}

#endif // PACKETCENTROID_H_INCLUDED