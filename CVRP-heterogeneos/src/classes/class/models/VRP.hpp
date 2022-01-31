#ifndef VRP_H_INCLUDED
#define VRP_H_INCLUDED

#pragma once

#include "Model.hpp"
class VRP final : public Model{
public:
    int N = 0;
    int K = 0;
    std::vector<Packet> packets;
    std::vector<Vehicle> vehicles;
    
    IloNumArray p;                              // pedidos
    IloArray <IloNumArray> d;                   // matrix_distance
    IloNumArray e;                              // custo veiculo
    IloBoolArray v;                             // veiculo ativado
    IloArray<IloBoolArray> w;                   // packet visited per vehicle
    IloNumArray Q;                              // carga maxima de um veiculo
    IloNumVarArray u;                           // aux_carga
    IloBoolVarArray y;                          // aux_dont return
    IloArray<IloBoolVarArray> z;                // vehicle leave packet
    IloArray <IloArray <IloBoolVarArray>> x;    // variavel de decisao
    
    VRP(
        IloArray <IloNumArray> output,
        std::vector<Packet> packets, 
        std::vector<Vehicle> vehicles
    );
    void createParams() override;
    void createVariables() override;
    void createFunctionObjetive() override;
    void createConstraints() override;
    Solution solve(int timeLimite) override;

    void renameVars();

    void relax_and_fix();
    void constraintDestiny();
    void constraintDriverGoToDestiny();
    void constraintBecame();
    void constraintDriverBecame();
    void constraintPacketSendByVehicle();
    void constraintPacketSolvedByVehicle();
    void constraintUseVehicles();
    void constraintWarrantOutflowDeposit();
    void constraintWarrantNoReturnDeposit();
    void constraintTotalVehicles();

    double distance_euclidian(Packet origin, Packet destiny);
};

#endif // VRP_H_INCLUDED