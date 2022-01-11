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
    std::vector<std::vector< double >> matrix_distance;
    std::vector<std::vector< double >> matrix_price;

    void calculate_matrix_distance();
    void calculate_matrix_price();
    double distance_euclidian(Packet origin, Packet destiny);
    CVRPSolution solve();
};