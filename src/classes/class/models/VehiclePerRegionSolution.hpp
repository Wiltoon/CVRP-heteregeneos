#ifndef VEHICLEPERREGION_H_INCLUDED
#define VEHICLEPERREGION_H_INCLUDED

#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <string>

#include <ilcplex/ilocplex.h>

#include "../Packet.hpp"
#include "../Vehicle.hpp"

class VehiclePerRegionSolution{
public:
    IloArray <IloNumArray> output;
    std::string message;
    std::vector<Packet> packets;
    std::vector<Vehicle> vehicles;
    int total_regions = 0;

    std::map<int,std::vector<Vehicle>> vehiclePerRegion;

    VehiclePerRegionSolution();
    VehiclePerRegionSolution(
        IloArray <IloNumArray> output,
        std::string message,
        std::vector<Packet> packets,
        std::vector<Vehicle> vehicles,
        int total_regions
    );
    VehiclePerRegionSolution(
        std::string message
    );
};

#endif // VEHICLEPERREGION_H_INCLUDED