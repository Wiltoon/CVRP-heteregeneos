#ifndef INSTANCE_H_INCLUDED
#define INSTANCE_H_INCLUDED

#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "..\src\classes\class\CVRP.hpp"


class Instance{
public:
    Instance() {

    }
    virtual CVRP readInput(
        std::string filename,
        std::string filevehicle,
        double alpha, 
        int N, int K) = 0;
    virtual void parse(
        std::ifstream& arquivo,
        std::string filevehicle, 
        CVRP& problem) = 0;
};

#endif // INSTANCE_H_INCLUDED