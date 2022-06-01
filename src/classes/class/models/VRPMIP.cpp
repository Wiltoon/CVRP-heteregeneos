#include <iostream>

#include "VRPMIP.hpp"
#include "../Packet.hpp"
#include "../Vehicle.hpp"

VRPMIP::VRPMIP(
        IloArray <IloNumArray> output,
        std::vector<Packet> packets, 
        std::vector<Vehicle> vehicles, 
        double **matriz,
        int region
    ){
    this->region = region;
    this->packets = packets;
    this->vehicles = vehicles;
    this->output = output;
    this->matriz = matriz;
    this->N = packets.size();
    this->K = vehicles.size();
    IloModel model(env);
    this->model = model;
}

void VRPMIP::createParams(){
    // Matriz de distancia
    IloArray <IloNumArray> d(env, N); 
    for (int i = 0; i < N; i++) {
        d[i] = IloNumArray(env, N);
    }
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++){
            if(i == j){
                d[i][j] = 999999.0;
            } else {
                d[i][j] = matriz[i][j];
            }
        }
    }
    // Carga dos Pacotes
    IloNumArray p(env, N);
    for(int i=0; i<N; i++){
        p[i] = packets[i].charge;
    }
    // Custo dos veículos
    IloNumArray e(env, K);
    // Pacote atendido pelo veiculo k w_kj
    IloArray<IloBoolArray> w(env, K); // DESTINOS (index j)
    // Ativação do veiculo k
    IloBoolArray v(env, K);
    // Carga maxima do veículo k
    IloNumArray Q(env, K);
    for (int k = 0; k < K; k++) {
        e[k] = vehicles[k].cust;
        w[k] = IloBoolArray(env, N);
        v[k] = 1;
        Q[k] = vehicles[k].charge_max;
    }
    this->d = d;
    this->p = p;
    this->e = e;
    this->v = v;
    this->w = w;
    this->Q = Q;
    for(int k = 0; k < K; k++){
        for (int j = 0; j < N; j++){
            this->w[k][j] = (this->managerPackets[k][j] > 0.9);
        }
    }
}

void VRPMIP::createVariables(){
    // Veículo k percorre o arco i->j
    IloArray <IloArray <IloBoolVarArray>> x(env, K);
    for (int k = 0; k < K; k++) {
        x[k] = IloArray <IloBoolVarArray>(env, N);				// Rotas atribuidas aos arcos (i,j)
        for (int i = 0; i < N; i++) {
            x[k][i] = IloBoolVarArray(env, N);
        }
    }
    // Veículo K chega do ponto j
    IloArray<IloBoolVarArray> y(env, K);
    // Veículo K sai do ponto i
    IloArray<IloBoolVarArray> z(env, K); // (index i)
    // Veículo K carrega U cargas até o ponto J
    IloArray<IloNumVarArray> u(env, K);
    for (int k = 0; k < K; k++){
        y[k] = IloBoolVarArray(env, N);
        z[k] = IloBoolVarArray(env, N);
        u[k] = IloNumVarArray(env, N, 0, vehicles[k].charge_max);// Variavel auxiliar para eliminicao de rota
    }
    this->x = x;
    this->y = y;
    this->z = z;
    this->u = u;
    renameVars();
}

void VRPMIP::renameVars(){
    char* char_u;
    char* char_y;
    for(int k = 0; k < K; k++){
        for(int i = 0; i < N; i++) {
            std::string nameu("u_" + std::to_string(k) + "_" + std::to_string(i));
            char_u = &nameu[0];
            u[k][i].setName(char_u);
        }
        for(int i = 0; i < N; i++){
            std::string namey("y_" + std::to_string(k) + "_" + std::to_string(i));
            char_y = &namey[0];
            y[k][i].setName(char_y);
        }
    }


    char* char_z;
    for(int k = 0; k < K; k++){
        for(int i = 0; i < N; i++) {
            std::string namez("z_" + std::to_string(k) + "_" + std::to_string(i));
            char_z = &namez[0];
            z[k][i].setName(char_z);
        }
    }

    char* char_x;
    for(int k = 0; k < K; k++){
        for(int i = 0; i < N; i++) {
            for(int j = 0; j < N; j++) {
                std::string namex("x_" + std::to_string(k) + "_" + std::to_string(i) + "_" + std::to_string(j));
                char_x = &namex[0];
                x[k][i][j].setName(char_x);
            }
        }
    }
}

void VRPMIP::createFunctionObjetive() {
    // criar a funcao objetivo no ambiente env e_k pode ser custo por km
    IloExpr fo(env);
    for (int k = 0; k < K; k++) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                fo += d[i][j] * x[k][i][j];
            }
        }
        fo += e[k] * v[k];
    }
    model.add(IloMinimize(env, fo));
    fo.end();
}

void VRPMIP::createConstraints(){

}

Solution VRPMIP::solve(int timeLimite) {
    // aqui deve resolver o problema VRP
    IloCplex cplex(model);
    IloNum objFO = IloInfinity;
    createParams();
    createVariables();
    createFunctionObjetive();
    createConstraints();
    VRPSolution o = mips(timeLimite, cplex);
    Solution sol = Solution(o);
    return sol;
}

VRPSolution VRPMIP::mip(int timeLimite, IloCplex & cplex){
    IloArray <IloArray <IloNumArray>> xSol = buildXSol();
    IloArray <IloNumArray> uSol = buildUSol();

    IloBool result = solveIterationMIP(timeLimite, cplex);
    if(result){
        assignTheSolutionsMIP(xSol, uSol, cplex);
    }
    else {
        std::cout << "Sem solução!";
    }
    VRPSolution vrp = VRPSolution(
        xSol, uSol, vehicles, packets
    );
    return vrp;
}