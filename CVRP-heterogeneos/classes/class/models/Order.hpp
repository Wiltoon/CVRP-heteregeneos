#ifndef ORDER_H_INCLUDED
#define ORDER_H_INCLUDED

#pragma once

#include "Model.hpp"

class Order final : public Model{
private:
    std::vector<Packet> packets_order;
public:
    void createParams() override;
    void createVariables() override;
    void createFunctionObjetive() override;
    void createConstraints() override;
    CVRPSolution solve() override;
};

#endif // ORDER_H_INCLUDED