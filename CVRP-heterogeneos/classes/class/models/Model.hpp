#pragma once

class Model{
    Parameters params;
    Variables vars;
    virtual createParams();
    virtual createVariables();
    virtual createConstraints();
    virtual solve()->CVRPSolution;
}