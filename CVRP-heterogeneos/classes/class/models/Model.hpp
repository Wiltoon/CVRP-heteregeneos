#pragma once

#include <ilcplex/ilocplex.h>
#include "../class/CVRPSolution.hpp"
class Model{
public:
    virtual void createParams() = 0;
    virtual void createVariables() = 0;
    virtual void createFunctionObjetive() = 0;
    virtual void createConstraints() = 0;
    virtual CVRPSolution solve() = 0;
};