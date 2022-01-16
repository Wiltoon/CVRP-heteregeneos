#pragma once

#include <ilcplex/ilocplex.h>
#include "../class/CVRPSolution.hpp"
class Model{
public:
    IloEnv env;
    IloModel model(env);
    virtual void createParams() = 0;
    virtual void createVariables() = 0;
    virtual void createFunctionObjetive() = 0;
    virtual void createConstraints() = 0;
    virtual Solution solve() = 0;
};