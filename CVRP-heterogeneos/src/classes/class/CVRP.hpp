#ifndef CVRP_H_INCLUDED
#define CVRP_H_INCLUDED

#pragma once

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

#include <ilcplex/ilocplex.h>

#include "../../KMeans.hpp"
#include "Packet.hpp"
#include "Vehicle.hpp"
#include "Solution.hpp"
#include "models/WhichVehicleProblem.hpp"
#include "models/VehiclePerRegionSolution.hpp"
#include "models/Order.hpp"
#include "models/VRP.hpp"

class CVRP {
private:
public:
    KMeans bestK;
    std::vector<Packet> packets;
    std::vector<Vehicle> vehicles;
    double** matrix_distance;
    double** matrix_price;

    CVRP();
    CVRP(int N);
    void calculate_matrix_distance(int N);
    double distance_euclidian(Packet origin, Packet destiny);
    Solution solveWithKmeans(
        int timeOrder, int timeVRP, int N, int K, double alpha);
    Solution solve(
        int timeOrder, int timeVRP, 
        std::vector<Vehicle> vehicles_used, 
        std::vector<Packet> packs, double alpha
    );
    double price_packet_per_vehicle(
        Packet& packet, Vehicle& vehicle, double alpha
    );
    void printerPackets(std::vector<Packet> packs);
    void printerVehicles(std::vector<Vehicle> vehicles);
    KMeans avaliateBestKmeans(std::vector<KMeans> possiblesKs);
    VehiclePerRegionSolution optimizeVehicles(KMeans kmeans);
};

#endif // CVRP_H_INCLUDED