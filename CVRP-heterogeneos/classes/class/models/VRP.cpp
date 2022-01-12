#include <iostream>

#include "VRP.hpp"
#include "../Packet.hpp"
#include "../Vehicle.hpp"

VRP::VRP(
    IloEnv env, 
    std::vector<Packet> packets, 
    int N, 
    std::vector<Vehicle> vehicles, 
    int K
){
    IloNumArray p(env, N);
    for(int i=0; i<N; i++){
        p[i] = packets[i].charge;
    }
    IloNumArray e(env, K);
    for (int i = 0; i < K; i++){
        e[i] = vehicles[i].cust;
    }
    IloArray <IloArray <IloBoolVarArray>> x(env, K);
    for (int k = 0; k < K; k++) {
        x[k] = IloArray <IloBoolVarArray>(env, N);				// Rotas atribuidas aos arcos (i,j)
        for (int i = 0; i < N; i++) {
            x[k][i] = IloBoolVarArray(env, N);
        }
    }
    IloBoolVarArray v(env, K);
    IloArray <IloNumArray> d(env, N);					// Matrizes de distancia e tempo entre pedido i ate j
    for (int i = 0; i < N; i++) {
        d[i] = IloNumArray(env, N);
    }
    IloBoolVarArray y(env, N);
    IloArray<IloBoolVarArray> w(env, K); // DESTINOS (index j)
    for (int k = 0; k < K; k++) {
        w[k] = IloBoolVarArray(env, N);
    }
    IloArray<IloBoolVarArray> z(env, K); // ORIGENS (index i)
    for (int k = 0; k < K; k++) {
        z[k] = IloBoolVarArray(env, N);
    }
    IloArray<IloNumVarArray> u(env, K);
    for (int k = 0; k < K; k++) {
        u[k] = IloNumVarArray(env, N, 0, 500);							// Variavel auxiliar para eliminicao de rota
    }
}

void VRP::createParams() override{
    // nomear todas os parametros de entrada
}

void VRP::createVariables() override{
    // nomear as variaveis de decisao
}

void VRP::createFunctionObjetive() override{
    // criar a funcao objetivo no ambiente env
}

void VRP::createConstraints() override{
    // criar as constraints
}

CVRPSolution solve() override{
    // aqui deve resolver o problema VRP
}

void VRP::relax_and_fix(){
    // construção do modelo relax and fix para resolver
}