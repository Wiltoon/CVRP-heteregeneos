#ifndef VRPMIP_H_INCLUDED
#define VRPMIP_H_INCLUDED

#pragma once

#include "Model.hpp"

class VRPMIP final : public Model{
public:
    int region = -1;        // index region
    int N = 0;      // index pedidos
    int K = 0;      // index vehicles 
    std::vector<Packet> packets;
    std::vector<Vehicle> vehicles;
    double **matriz;
    
    IloArray <IloNumArray> d;       // matrix_distance
    IloNumArray p;                  // pedidos
    IloNumArray e;                  // custo dos veiculos
    IloBoolArray v;                 // veiculos ativos
    IloArray<IloBoolArray> w;       // Pacote j atendido por k
    IloNumArray Q;                  // Carga maxima do veiculo K

    IloArray <IloNumArray> managerPackets;

    IloArray<IloNumVarArray> u;                 // aux_carga
    IloArray<IloBoolVarArray> y;                // aux_dont return
    IloArray<IloBoolVarArray> z;                // vehicle leave packet
    IloArray <IloArray <IloBoolVarArray>> x;    // variavel de decisao

    VRPMIP(
        IloArray <IloNumArray> output,
        std::vector<Packet> packets, 
        std::vector<Vehicle> vehicles, 
        double **matriz,
        int region
    );
    void createParams() override;
    void createVariables() override;
    void createFunctionObjetive() override;
    void createConstraints() override;
    Solution solve(int timeLimite) override;

    void renameVars();

    VRPSolution mip(int timeLimite, IloCplex & cplex);
    void assignTheSolutionsMIP(
        IloArray <IloArray <IloNumArray>> & xSol,
        IloArray <IloNumArray> & uSol,
        IloCplex & cplex
    );
    IloBool solveIterationMIP(
        int tempo,
        IloCplex & cplex
    );

    
    IloArray <IloNumArray> buildUSol();
    IloArray <IloArray <IloNumArray>> buildXSol();
    
    void constraintLeaveCustomer();
    void constraintArriveCustomer();
    void constraintFlow();
    void constraintDeposit();
    void constraintMTZ();
}

#endif // VRPMIP_H_INCLUDED