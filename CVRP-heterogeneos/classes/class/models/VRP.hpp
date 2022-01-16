#pragma once

#include "Model.hpp"
class VRP final : public Model{
public:
    int N = 0;
    int K = 0;
    std::vector<Packet> packets;
    std::vector<Vehicle> vehicles;
    
    IloNumArray p;                              // pedidos
    IloArray <IloArray <IloBoolVarArray>> x;    // variavel de decisao
    IloArray <IloNumArray> d;                   // matrix_distance
    IloNumVarArray u;                           // aux_carga
    IloNumArray e;
    IloBoolVarArray v;
    IloBoolVarArray y;
    IloArray<IloBoolVarArray> z;
    IloArray<IloBoolArray> w;
    IloNumArray Q;                              // carga maxima de um veiculo
    
    VRP(
        IloEnv env, 
        std::vector<Packet> packets, 
        int N, 
        std::vector<Vehicle> vehicles, 
        int K
    );
    void createParams() override;
    void createVariables() override;
    void createFunctionObjetive() override;
    void createConstraints() override;
    Solution solve() override;
    void relax_and_fix();
};