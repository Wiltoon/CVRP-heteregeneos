#ifndef VRP_H_INCLUDED
#define VRP_H_INCLUDED

#pragma once

#include "Model.hpp"

int DEPOSIT = 0;
int TIME_MAX = 30;
int SOMADOR_TIME = 6;
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

    void relax_and_fix(int time);
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
    IloArray <IloNumArray> buildUsol();
    IloArray <IloArray <IloNumArray>> buildXSol();
    IloArray <IloArray <IloExtractableArray>> relaxAll();
    int pathsToFix();
    void printerVector(std::string name, std::vector<int> elements);
    void removeRelaxationToVisit(
        IloArray <IloArray<IloExtractableArray>> relaxa,
        std::vector<int> visitar
    );
    void VRP::assignTheSolutions(
        IloArray <IloArray <IloNumArray>> xSol,
        IloArray <IloNumArray> uSol
    )
    IloBool solveIteration(int iteration, int tempo);
    void fixVariables(
        IloArray <IloArray <IloNumArray>> xSol,
        std::vector <int> visitar,
        std::vector <int> visitado
    );
    void fixXYZ(std::vector <int> visitar, int check, int k, int i);
    void buildNewConstraint(int entrega);
    void fixedColumn(
        IloArray <IloArray <IloNumArray>> sol,
        std::vector <int> visitar,
        int check, 
        int i, int k    
    );
    void fixedRow(
        IloArray <IloArray <IloNumArray>> sol,
        std::vector <int> visitar,
        int check, 
        int i, int k 
    );
    void fixedSimetrics(
        IloArray <IloArray <IloNumArray>> sol,
        std::vector <int> visitar,
        int check,int i
    );
    bool findElementInVector(
        std::vector <int> vector,
        int i
    );
    void calculateWhoToFix(
        IloArray <IloArray <IloNumArray>> xSol,
        std::vector <int> visitar,
        std::vector <int> visitado,
        std::vector<int> auxvisitar,
        int check, int k
    );
};

#endif // VRP_H_INCLUDED