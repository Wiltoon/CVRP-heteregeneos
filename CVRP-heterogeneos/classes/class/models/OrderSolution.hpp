#ifndef ORDERSOLUTION_H_INCLUDED
#define ORDERSOLUTION_H_INCLUDED

#pragma once 

#include <stdio.h>
#include <iostream>
#include <string>
#include <ilcplex/ilocplex.h>

class OrderSolution {
public:
    IloArray <IloNumArray> output;
    std::string message;

    OrderSolution() {

    }
    OrderSolution(IloArray <IloNumArray> output, std::string message);
    OrderSolution(std::string messageFail);
    void printerOrderSolution(int N, int K);
};

#endif // ORDERSOLUTION_H_INCLUDED