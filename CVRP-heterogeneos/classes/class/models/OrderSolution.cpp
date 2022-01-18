#include "OrderSolution.hpp"

OrderSolution::OrderSolution(IloArray <IloNumArray> output, std::string message){
    this->output = output;
    this->message = message;
}

OrderSolution::OrderSolution(std::string messageFail){
    this->message = messageFail;
}

void OrderSolution::printerOrderSolution(int N, int K){
    for(int k = 0; k < K; k++){
        for(int i = 0; i < N; i++){
            std::cout << output[k][i] << ", ";
        }
        std::cout << std::endl;
    }
}