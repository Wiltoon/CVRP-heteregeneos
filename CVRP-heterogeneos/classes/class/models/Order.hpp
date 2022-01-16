#ifndef ORDER_H_INCLUDED
#define ORDER_H_INCLUDED

#pragma once

#include "Model.hpp"

class Order final : public Model{
private:
    std::vector<Packet> packets_order;
public:
    IloArray<IloBoolArray> p;   // param calculado anteriormente e entregue como entrada do problema
    IloArray<IloBoolVarArray> w;   // variavel booleana indica se o packet j foi entregue pelo veicul k
    IloArray <IloArray <IloNumArray>> c;  // param preço ao percorrer o caminho i e j pelo veiculo k
    IloNumArray q;    // param vector indica a carga presente no packet i
    IloNumArray Qmax; // param vetor indica carga maxima do veiculo k

    void createParams() override;
    void createVariables() override;
    void createFunctionObjetive() override;
    void createConstraints() override;
    Solution solve() override;
};

#endif // ORDER_H_INCLUDED