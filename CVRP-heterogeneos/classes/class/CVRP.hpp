#pragma once
#include <iostream>
#include <vector>
#include "Packet.hpp"
#include "Vehicle.hpp"
#include "..\..\src\CVRPSolution.hpp"

class CVRP{
public:
    std::vector<Packet> packets;
    std::vector<Vehicle> vehicles;
    std::vector<std::vector< double >> matrix_distance;
    std::vector<std::vector< double >> matrix_price;


    CVRP();
    void calculate_matrix_distance(std::vector<Packet> packtes);
    void calculate_matrix_price();
    double distance_euclidian(Packet origin, Packet destiny);
    CVRPSolution solve();
};