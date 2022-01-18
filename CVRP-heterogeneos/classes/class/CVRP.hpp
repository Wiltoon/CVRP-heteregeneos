#ifndef CVRP_H_INCLUDED
#define CVRP_H_INCLUDED

#pragma once
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include "math.h"

#include "Packet.hpp"
#include "Vehicle.hpp"
#include "../class/Solution.hpp"
#include "NeighborPacket.hpp"

class CVRP {
private:
    static bool crescentePackets(
        const NeighborPacket& p1,const NeighborPacket& p2);
public:
    std::vector<Packet> packets;
    std::vector<Vehicle> vehicles;
    double** matrix_distance;
    double** matrix_price;

    CVRP(int num_pedidos, int num_vehicles);
    void calculate_matrix_distance();
    void calculate_matrix_price(double alpha);
    double distance_euclidian(Packet origin, Packet destiny);
    Solution solve(int timeOrder, int timeVRP);

    double price_packet_per_vehicle(
        Packet& packet, Vehicle& vehicle, double alpha
    );
    void collectPacketsAround(Vehicle vehicle);
    std::vector<NeighborPacket> sortPacketsAroundPacket(Packet pac);
};

#endif // CVRP_H_INCLUDED