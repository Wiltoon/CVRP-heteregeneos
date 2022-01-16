#ifndef ORDERSOLUTION_H_INCLUDED
#define ORDERSOLUTION_H_INCLUDED

#pragma once 

class OrderSolution : public Solution{
public:
    IloArray <IloNumArray> output;

    Solution getOutput() override; 
}

#endif // ORDERSOLUTION_H_INCLUDED