#ifndef INSTANCE_H_INCLUDED
#define INSTANCE_H_INCLUDED

#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "..\classes\class\CVRP.hpp"


class Instance{
public:
    virtual CVRP readInput(std::string filename, double alpha) = 0;
    virtual void parse(std::ifstream& arquivo, CVRP& problem) = 0;
};

#endif // INSTANCE_H_INCLUDED