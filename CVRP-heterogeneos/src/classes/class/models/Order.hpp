#ifndef ORDER_H_INCLUDED
#define ORDER_H_INCLUDED

#pragma once

#include <vector>
#include <cmath>
#include <algorithm>

#include "Model.hpp"
#include "../Vehicle.hpp"
#include "../Packet.hpp"
#include "../NeighborPacket.hpp"

class Order final : public Model{
private:
    std::vector<Packet> packets_order;
    std::vector<Vehicle> vehicles;
    int N;
    int K;
    int region;

    static bool crescentePackets(
        const NeighborPacket& p1, const NeighborPacket& p2);
public:
    double** matrix_prices;
    IloArray <IloNumArray> mp;   // param calculado anteriormente e entregue como entrada do problema
    IloNumArray q;    // param vector indica a carga presente no packet i
    IloNumArray Qmax; // param vetor indica carga maxima do veiculo k
    IloArray <IloNumArray> e;
    
    IloArray <IloBoolVarArray> w;   // variavel booleana indica se o packet j foi entregue pelo veicul k
    IloBoolVarArray z;              // indica se veiculo foi usado

    Order(
        std::vector<Packet> packets_order_,
        std::vector<Vehicle> vehicles_,
        double alpha, int region
    );

    void createParams() override;
    void createVariables() override;
    void createFunctionObjetive() override;
    void createConstraints() override;
    Solution solve(int timeLimite) override;

    void renameVars();
    void constraintDestiny();
    void constraintCapacitedVehicle();

    OrderSolution outputOrder(IloCplex& cplex);
    double** calculate_matrix_price(double alpha, int N, int K);
    double price_packet_per_vehicle(
        Packet& packet, Vehicle& vehicle, double alpha
    );
    std::vector<NeighborPacket> sortPacketsAroundPacket(Packet pac);
    double distance_euclidian(Packet origin, Packet destiny);
};

#endif // ORDER_H_INCLUDED