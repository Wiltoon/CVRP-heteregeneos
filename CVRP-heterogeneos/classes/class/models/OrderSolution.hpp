#ifndef ORDERSOLUTION_H_INCLUDED
#define ORDERSOLUTION_H_INCLUDED

#pragma once 

#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include <ilcplex/ilocplex.h>

#include "../../class/Packet.hpp"
#include "../../class/Vehicle.hpp"

class OrderSolution {
public:
    IloArray <IloNumArray> output;
    std::vector<Packet> packets;
    std::vector<Vehicle> vehicles;
    std::string message;

    OrderSolution() {

    }
    OrderSolution(
        IloArray <IloNumArray> output, 
        std::string message, 
        std::vector<Packet> packets,
        std::vector<Vehicle> vehicles
        );
    OrderSolution(IloArray <IloNumArray> output, std::string message);
    OrderSolution(std::string messageFail);
    void printerOrderSolution(int N, int K);
};

#endif // ORDERSOLUTION_H_INCLUDED