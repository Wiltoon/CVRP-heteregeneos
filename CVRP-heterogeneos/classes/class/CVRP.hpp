#pragma once
#include <iostream>
#include <vector>
#include "math.h"
#include "Packet.hpp"
#include "Vehicle.hpp"
#include "..\..\src\CVRPSolution.hpp"

class CVRP{
public:
    std::vector<Packet> packets;
    std::vector<Vehicle> vehicles;
    double **matrix_distance;
    double **matrix_price;

    CVRP(int num_pedidos, int num_vehicles);
    void calculate_matrix_distance();
    void calculate_matrix_price();
    double distance_euclidian(Packet origin, Packet destiny);
    CVRPSolution solve();

    double price_packet_per_vehicle(Packet& packet, Vehicle& vehicle);
    void collectPacketsAround(Vehicle vehicle);
    Packet decrescentePackets(Packet p1, Packet p2);
    std::vector<Packet> sortPacketsAroundPacket(Packet pac);
};