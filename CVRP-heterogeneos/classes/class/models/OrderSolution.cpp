#include "OrderSolution.hpp"

OrderSolution::OrderSolution(IloArray <IloNumArray> output, std::string message){
    this->output = output;
    this->message = message;
}