#ifndef WHICHVEHICLEPROBLEM_H_INCLUDED
#define WHICHVEHICLEPROBLEM_H_INCLUDED

#pragma once

#include <cmath>
#include <vector>

#include "Model.hpp"
#include "../../../KMeans.hpp"
#include "VehiclePerRegionSolution.hpp"
#include "../Vehicle.hpp"
#include "../Packet.hpp"

class WhichVehicleProblem final : public Model{
private: // apenas parametros de entrada do problema
    KMeans kmeans; //regioes
    std::vector<Packet> packet_total;
    std::vector<Vehicle> vehicles;
public: // os parametros e variaveis criados 
    IloNumArray R;
    IloNumArray d;
    IloNumArray c;
    IloNumArray Qm;

    IloArray <IloBoolVarArray> x;

    WhichVehicleProblem(
        KMeans kmeans, 
        std::vector<Packet> packets,
        std::vector<Vehicle> vehicles
    );
    KMeans getKmeans();
    std::vector<Packet> getPacketsTotal();
    std::vector<Vehicle> getVehicles();

    void createParams() override;
    void createVariables() override;
    void createFunctionObjetive() override;
    void createConstraints() override;
    Solution solve(int timeLimite) override;

    void renameVars();
    void constraintRegionCapacity();
    void constraintJustAVehicleIntoRegion();
    int capacityTotalRegion(Cluster region);
    double distanceRegion(Cluster region);

    VehiclePerRegionSolution vehiclesPerRegion(IloCplex& cplex);
};

#endif // WHICHVEHICLEPROBLEM_H_INCLUDED

