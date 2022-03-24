#ifndef VRP_H_INCLUDED
#define VRP_H_INCLUDED

#pragma once

#include "Model.hpp"
#include <vector>       // std::vector

class VRP final : public Model{
public:

    int DEPOSIT = 0;
    int TIME_MAX = 30;
    int SOMADOR_TIME = 6;
    int region = -1;
    int N = 0;
    int K = 0;
    int knn = 0;
    std::vector<Packet> packets;                // DEPOSITO INCLUIDO
    std::vector<Vehicle> vehicles;
    double** matrix_distance;
    IloArray <IloNumArray> output;
    
    IloArray <IloNumArray> d;                   // matrix_distance
    IloNumArray p;                              // pedidos
    IloNumArray e;                              // custo veiculo
    IloNumArray Q;                              // carga maxima de um veiculo

    IloBoolVarArray v;                             // veiculo ativado
    IloArray<IloNumVarArray> u;                 // aux_carga
    IloArray<IloBoolVarArray> y;                // aux_dont return
    IloArray<IloBoolVarArray> w;                   // packet visited per vehicle
    IloArray<IloBoolVarArray> z;                // vehicle leave packet
    IloArray <IloArray <IloBoolVarArray>> x;    // variavel de decisao
    
    VRP(
        IloArray <IloNumArray> output,
        std::vector<Packet> packets, 
        std::vector<Vehicle> vehicles, 
        int region
    );
    VRP(
        std::vector<Packet> packets, 
        std::vector<Vehicle> vehicles
    );
    VRP(
        std::vector<Packet> packets, 
        std::vector<Vehicle> vehicles, 
        double** matrix_distance,
        int knn
    );
    void createParams() override;
    void createVariables() override;
    void createFunctionObjetive() override;
    void createConstraints() override;
    Solution solve(int timeLimite) override;
    Solution solve(int timeLimite, std::string nameInstance);
    Solution solveLCR(int timeLimite);

    void renameVars();

    VRPSolution relax_and_fix(int time, IloCplex & cplex);
    VRPSolution relax_and_fix(int time, IloCplex & cplex, std::string nameInstance);
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
    void constraintMTZ();
    void constraintLimitMTZ();

    double distance_euclidian(Packet origin, Packet destiny);
    IloArray <IloNumArray> buildUSol();
    IloArray <IloArray <IloNumArray>> buildXSol();
    IloArray <IloArray <IloExtractableArray>> relaxAll();
    bool pathsToFix(std::vector<int> visited);
    void printerVector(std::string name, std::vector<int> elements);
    void removeRelaxationToVisit(
        IloArray <IloArray<IloExtractableArray>> relaxa,
        std::vector<int> visitar
    );
    bool packetIsNeighbor(int i, int j);
    void selectListCandidatesRestrict();
    void selectListCandidatesRestrict(std::vector <int> to_visit, std::vector <int> visited);
    void assignTheSolutions(
        IloArray <IloArray <IloNumArray>> & xSol,
        IloArray <IloNumArray> & uSol,
        std::vector <int> visitar,
        IloCplex & cplex
    );
    IloBool solveIteration(
        int iteration, 
        int tempo,
        IloCplex & cplex
    );
    void fixVariables(
        IloArray <IloArray <IloNumArray>> xSol,
        std::vector <int> & visitar,
        std::vector <int> & visitado
    );
    void fixXYZ(
        std::vector <int> visitados,
        std::vector <int> visitar, 
        int check, int k, int i
    );
    void toFixY(std::vector <int> visitados, int k, int l);
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
        int i,
        std::vector <int> vetor
    );
    void calculateWhoToFix(
        IloArray <IloArray <IloNumArray>> xSol,
        std::vector <int> & visitar,
        std::vector <int> & visitado,
        std::vector <int> & auxvisitar,
        int check, int k
    );
};

#endif // VRP_H_INCLUDED