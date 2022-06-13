#ifndef CVRP_H_INCLUDED
#define CVRP_H_INCLUDED

#pragma once

#include <iostream>
#include <thread>
#include <vector>
#include <cmath>
#include <algorithm>
#include <time.h>
#include <utility>

#include <ilcplex/ilocplex.h>
#include <curl/curl.h>

#include "../../KMeans.hpp"
#include "Packet.hpp"
#include "Vehicle.hpp"
#include "Solution.hpp"
#include "models/WhichVehicleProblem.hpp"
#include "models/VehiclePerRegionSolution.hpp"
#include "models/Order.hpp"
#include "models/CCP.hpp"
#include "models/VRP.hpp"
#include "models/VRPMIP.hpp"


class CVRP {
private:
public:
    std::vector<Solution> clusters_solved;
    KMeans bestK;
    std::vector<Packet> packets;
    std::vector<Vehicle> vehicles;
    double** matrix_distance;
    double** matrix_price;
    typedef struct Arguments {
        VehiclePerRegionSolution mapRegion;
        int region;
        int timeOrder;
        int timeVRP;
        double alpha;
    } Arg;
    typedef struct ArgumentsSol {
        OrderSolution mapW;
        int region;
        int timeOrder;
        int timeVRP;
        double alpha;
    } ArgS;
    CVRP();
    CVRP(int N);
    void calculate_matrix_distance(int N);
    void calculate_matrix_distance(int N, bool loggibud);
    void solveRegion(Arg args);
    void solveRegionMIP(Arg args, double **matrix_distance);
    void solveRegionMIP(ArgS args, double **matrix_distance);
    double distance_euclidian(Packet origin, Packet destiny);
    double solveKmeansParallel(
        std::string fileKmeans,
        int timeOrder,
        int timeVRP,
        double alpha,
        std::string nameInstance
    );
    double solveKmeansSeriable(
        std::string fileKmeans,
        int timeOrder,
        int timeVRP,
        double alpha,
        std::string nameInstance
    );
    double solveKmeansSeriableMIP(
        std::string fileKmeans,
        int timeOrder,
        int timeVRP,
        double alpha,
        std::string nameInstance,
        double **matrix_distance
    );
    double solveCCPSeriableMIP(
        std::string fileKmeans,
        int timeOrder,
        int timeVRP,
        double alpha, 
        std::string nameInstance,
        double **matrix_distance
    );
    std::vector<Vehicle> vehiclesUsed(ArgS args);
    std::vector<Packet> packetsVisited(ArgS args);
    void solveWithKmeans(
        int timeOrder, int timeVRP, int N, int K, double alpha);
    Solution solveWithKmeans(
        int timeOrder, int timeVRP, 
        int N, int K, double alpha,
        std::string nameInstance);
    Solution solve(
        int timeOrder, int timeVRP, int regiao,
        std::vector<Vehicle> vehicles_used, 
        std::vector<Packet> packs, double alpha
    );
    Solution solveMIP(
        int timeOrder, int timeVRP, int regiao,
        std::vector<Vehicle> vehicles_used, double **matrix_distance,
        std::vector<Packet> packs, double alpha
    );
    Solution solveMIP(ArgS args, double **matrix_distance);
    Solution solve(
        int timeOrder, int timeVRP, int regiao,
        std::vector<Vehicle> vehicles_used, 
        std::vector<Packet> packs, double alpha, std::string nameInstance
    );
    double price_packet_per_vehicle(
        Packet& packet, Vehicle& vehicle, double alpha
    );
    void printerPackets(std::vector<Packet> packs);
    void printerVehicles(std::vector<Vehicle> vehicles);
    void outputJson(
        std::string city,
        std::vector<Solution> solutions,
        std::string nameInstance,
        double time_execution
    );
    KMeans parseKmeans(std::string fileKmeans);
    KMeans avaliateBestKmeans(std::vector<KMeans> possiblesKs);
    VehiclePerRegionSolution optimizeVehicles(KMeans kmeans);
};

#endif // CVRP_H_INCLUDED