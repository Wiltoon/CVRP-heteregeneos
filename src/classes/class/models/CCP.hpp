#ifndef CCP_H_INCLUDED
#define CCP_H_INCLUDED

#pragma once

#include "Model.hpp"
#include "../Vehicle.hpp"
#include "../Packet.hpp"

class CCP final: public Model {
private:
    std::vector<Packet> deliveries;
    std::vector<Vehicle> vehicles;
    int K;
    int N;
public:
    // Packets
    double** matrix_distance;
    IloArray <IloNumArray> d;       // matrix_distance
    IloNumArray p;       // capacidade de um pacote      
    // Vehicles    
    IloNumArray Qr;      // capacidade maxima de um veiculo R
    IloNumArray c;       // custo do veiculo R

    IloArray <IloBoolVarArray> w; // R, J

    CCP(
        std::vector<Packet> packets_,
        std::vector<Vehicle> vehicles_used_,
        double** matrix_distance_,
    );        
    void createParams() override;
    void createVariables() override;
    void createFunctionObjetive() override;
    void createConstraints() override;
    Solution solve(int timeLimite) override;

    void renameVars();
    void constraintSeparatedPackets();
    void constraintMaxCapacited();
    void constraintIncludedDeposit();

    IloArray <IloNumArray> buildWSol();

    static double euclidean(Point p1, Point p2);
    void generate_solution(std::string nameFile);
};