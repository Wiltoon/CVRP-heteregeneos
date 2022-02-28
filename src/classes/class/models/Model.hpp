#ifndef MODEL_H_INCLUDED
#define MODEL_H_INCLUDED

#pragma once

#include <iostream>
#include <ilcplex/ilocplex.h>
#include <stdlib.h>
#include "../Solution.hpp"

class Model{
public:
    IloEnv env;
    IloModel model;
    virtual void createParams() = 0;
    virtual void createVariables() = 0;
    virtual void createFunctionObjetive() = 0;
    virtual void createConstraints() = 0;
    virtual Solution solve(int timeLimite) = 0;
};

#endif // MODEL_H_INCLUDED