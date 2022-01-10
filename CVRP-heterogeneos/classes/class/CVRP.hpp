#pragma once
#include <iostream>
#include <vector>
#include "Packet.hpp"
#include "Vehicle.hpp"

class CVRP{
    std::vector<Packet> packets;
    std::vector<Vehicle> vehicles;
    std::vector<std::vector< double >> matrix_distance;
    std::vector<std::vector< double >> matrix_price;

    
}