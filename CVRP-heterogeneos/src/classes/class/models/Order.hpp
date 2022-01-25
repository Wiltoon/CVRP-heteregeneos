#ifndef ORDER_H_INCLUDED
#define ORDER_H_INCLUDED

#pragma once

#include <vector>
#include "Model.hpp"
#include "../Vehicle.hpp"
#include "../Packet.hpp"

class Order final : public Model{
private:
    std::vector<Packet> packets_order;
    std::vector<Vehicle> vehicles;
    double** matrix_prices;
    int N;
    int K;
public:
    IloArray <IloNumArray> mp;   // param calculado anteriormente e entregue como entrada do problema
    IloNumArray q;    // param vector indica a carga presente no packet i
    IloNumArray Qmax; // param vetor indica carga maxima do veiculo k
    IloArray <IloNumArray> e;
    
    IloArray <IloBoolVarArray> w;   // variavel booleana indica se o packet j foi entregue pelo veicul k
    IloBoolVarArray z;              // indica se veiculo foi usado

    Order(
        std::vector<Packet> packets_order_, 
        std::vector<Vehicle> vehicles_,
        double** matrix_price_,
        int N_,
        int K_
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
};

#endif // ORDER_H_INCLUDED