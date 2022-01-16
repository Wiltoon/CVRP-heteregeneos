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
    this->env = env;
    this->packets = packets;
    this->vehicles = vehicles;
    this->N = N;
    this->K = K;
}

void VRP::createParams() override{
    // nomear todas os parametros de entrada
    IloArray <IloNumArray> d(env, N);					// Matrizes de distancia e tempo entre pedido i ate j
    for (int i = 0; i < N; i++) {
        d[i] = IloNumArray(env, N);
    }
    IloNumArray p(env, N);
    for(int i=0; i<N; i++){
        p[i] = packets[i].charge;
    }
    IloNumArray e(env, K);
    for (int i = 0; i < K; i++){
        e[i] = vehicles[i].cust;
    }
    IloArray<IloBoolArray> w(env, K); // DESTINOS (index j)
    for (int k = 0; k < K; k++) {
        w[k] = IloBoolArray(env, N);
    }
}   

void VRP::createVariables() override{
    // nomear as variaveis de decisao
    IloArray <IloArray <IloBoolVarArray>> x(env, K);
    for (int k = 0; k < K; k++) {
        x[k] = IloArray <IloBoolVarArray>(env, N);				// Rotas atribuidas aos arcos (i,j)
        for (int i = 0; i < N; i++) {
            x[k][i] = IloBoolVarArray(env, N);
        }
    }
    IloBoolVarArray v(env, K);
    IloBoolVarArray y(env, N);
    
    IloArray<IloBoolVarArray> z(env, K); // ORIGENS (index i)
    for (int k = 0; k < K; k++) {
        z[k] = IloBoolVarArray(env, N);
    }
    IloArray<IloNumVarArray> u(env, K);
    for (int k = 0; k < K; k++) {
        u[k] = IloNumVarArray(env, N, 0, vehicles[k].charge_max);// Variavel auxiliar para eliminicao de rota
    }
}

void VRP::createFunctionObjetive() override{
    // criar a funcao objetivo no ambiente env
    IloExpr fo(env);
    for (int k = 0; k < K; k++) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                fo += d[i][j] * x[k][i][j];
            }
        }
        fo += e[k] * v[k];
    }
    modelo.add(IloMinimize(env, fo));
    fo.end();
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