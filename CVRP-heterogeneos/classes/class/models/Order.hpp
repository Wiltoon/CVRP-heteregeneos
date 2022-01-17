#ifndef ORDER_H_INCLUDED
#define ORDER_H_INCLUDED

#pragma once

#include "Model.hpp"
#include "Vehicle.hpp"
#include "Packet.hpp"

class Order final : public Model{
private:
    std::vector<Packet> packets_order;
    std::vector<Vehicle> vehicles;
public:
    IloArray <IloNumArray> mp;   // param calculado anteriormente e entregue como entrada do problema
    IloNumArray q;    // param vector indica a carga presente no packet i
    IloNumArray Qmax; // param vetor indica carga maxima do veiculo k
    
    IloArray <IloBoolVarArray> w;   // variavel booleana indica se o packet j foi entregue pelo veicul k

    void createParams() override;
    void createVariables() override;
    void createFunctionObjetive() override;
    void createConstraints() override;
    Solution solve() override;

    void renameVars();
    void constraintDestiny();
    void constraintCapacitedVehicle();
};

#endif // ORDER_H_INCLUDED