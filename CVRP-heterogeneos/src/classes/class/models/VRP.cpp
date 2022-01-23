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

void VRP::createParams() {
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

void VRP::createVariables() {
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

void VRP::createFunctionObjetive() {
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
    model.add(IloMinimize(env, fo));
    fo.end();
}


void VRP::createConstraints() {
    // criar as constraints
    constraintDestiny();
    constraintDriverGoToDestiny();
    constraintBecame();
    constraintDriverBecame();
    constraintPacketSendByVehicle();
    constraintPacketSolvedByVehicle();
    constraintUseVehicles();
    constraintWarrantOutflowDeposit();
    constraintWarrantNoReturnDeposit();
    constraintTotalVehicles();
}

Solution VRP::solve(int timeLimite) {
    // aqui deve resolver o problema VRP
    OrderSolution in = OrderSolution();
    VRPSolution o = VRPSolution();
    Solution sol = Solution(in, o);
    return sol;
}

void VRP::relax_and_fix(){
    // construção do modelo relax and fix para resolver
}

void VRP::constraintDestiny(){
    IloConstraintArray cons_destino(env);
    for (int j = 1; j < N; j++) {
        IloExpr restDest(env);
        char* namevar;
        std::string name("alguemChegaAoCliente_" + std::to_string(j));
        namevar = &name[0];
        for (int k = 0; k < K; k++) {
            restDest += w[k][j];
        }
        IloConstraint consRestDest = (restDest == 1);
        consRestDest.setName(namevar);
        model.add(consRestDest);
        cons_destino.add(consRestDest);
        restDest.end();
    }
}

void VRP::constraintDriverGoToDestiny(){
    IloConstraintArray cons_destino_veiculo(env);
    for (int k = 0; k < K; k++) {
        IloExpr restDestDriver(env);
        char* namevarDD;
        std::string name("veiculo_dest_" + std::to_string(k));
        namevarDD = &name[0];
        for (int i = 0; i < N; i++) {
            restDestDriver += w[k][i];
        }
        IloConstraint consRestOriginDriver = (restDestDriver <= v[k] * N);
        consRestOriginDriver.setName(namevarDD);
        model.add(consRestOriginDriver);
        cons_destino_veiculo.add(consRestOriginDriver);
        restDestDriver.end();
    }
}

void VRP::constraintBecame(){
    IloConstraintArray cons_chegada(env);
    for (int i = 1; i < N; i++) {
        IloExpr restOrig(env);
        char* namevar;
        std::string name("alguemSaiDoCliente[" + std::to_string(i) + "]");
        namevar = &name[0];
        for (int k = 0; k < K; k++) {
            restOrig += z[k][i];
        }
        IloConstraint consRestOrigin = (restOrig + y[i] == 1);
        consRestOrigin.setName(namevar);
        model.add(consRestOrigin);
        cons_chegada.add(consRestOrigin);
        restOrig.end();
    }
}

void VRP::constraintDriverBecame(){
    IloConstraintArray cons_chegada_veiculo(env);
    for (int k = 0; k < K; k++) {
        IloExpr restOrigDriver(env);
        char* namevarD;
        std::string name("veiculo_origin_" + std::to_string(k));
        namevarD = &name[0];
        for (int i = 0; i < N; i++) {
            restOrigDriver += z[k][i];
        }
        IloConstraint consRestOriginDriver = (restOrigDriver <= v[k] * N);
        consRestOriginDriver.setName(namevarD);
        model.add(consRestOriginDriver);
        cons_chegada_veiculo.add(consRestOriginDriver);
        restOrigDriver.end();
    }
}

void VRP::constraintPacketSendByVehicle(){
    IloConstraintArray cons_array_chegada(env);
    for (int k = 0; k < K; k++) {
        for (int j = 1; j < N; j++) {
            IloExpr restChegada(env); 
            char* namevar;
            std::string name("chegadaNoCliente_" + std::to_string(j) + "_peloVehicle_" + std::to_string(k));
            namevar = &name[0];
            for (int i = 0; i < N; i++) {
                if (i != j) {
                    restChegada += x[k][i][j];
                }
            }
            IloConstraint consRestChegada = (restChegada == w[k][j]);
            consRestChegada.setName(namevar);
            model.add(consRestChegada);
            cons_array_chegada.add(consRestChegada);
            restChegada.end();
        }
    }
}

void VRP::constraintPacketSolvedByVehicle(){
    IloConstraintArray cons_array_saida(env);
    for (int k = 0; k < K; k++) {
        for (int i = 0; i < N; i++) {
            IloExpr restSaida(env);
            char* namevar;
            std::string name("saidaDoCliente_" + std::to_string(i) + "_peloVehicle_" + std::to_string(k));
            namevar = &name[0];
            for (int j = 0; j < N; j++) {
                if (j != i) {
                    restSaida += x[k][i][j];
                }
            }
            IloConstraint consRestSaida = (restSaida == z[k][i]);
            consRestSaida.setName(namevar);
            model.add(consRestSaida);
            cons_array_saida.add(consRestSaida);
            restSaida.end();
        }
    }
}

void VRP::constraintUseVehicles(){
    IloConstraintArray cons_array_veiculos3(env);
    for (int k = 0; k < K; k++) {
        IloExpr restVeiX(env);
        char* namevar;
        std::string name("veiculo_" + std::to_string(k) + "_utilizado");
        namevar = &name[0];
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                restVeiX += x[k][i][j];
            }
        }
        IloConstraint consRestVei = (restVeiX <= v[k]*N);
        consRestVei.setName(namevar);
        model.add(consRestVei);
        cons_array_veiculos3.add(consRestVei);
        restVeiX.end();
    }
}

void VRP::constraintWarrantOutflowDeposit(){
    IloConstraintArray cons_deposit(env);
    IloExpr depCli(env);
    IloExpr depClient(env);
    int qntMinVei = 0;	
    for (int k = 0; k < K; k++) {
        depClient += z[k][0];
        for (int i = 1; i < N; i++) {
            depCli += x[k][0][i];
        }
    }
    IloConstraint depositToClient = (depCli == qntMinVei);
    IloConstraint depositToClient2 = (depClient == qntMinVei);
    depositToClient.setName("saida_Deposito");
    depositToClient2.setName("saida_Deposito2");
    depCli.end();
    depClient.end();
}

void VRP::constraintWarrantNoReturnDeposit(){
    IloExpr cliDep(env);
    IloExpr clientDep(env);
    IloConstraintArray cons_deposit(env);
    int qntMinVei = 0;	
    for (int k = 0; k < K; k++) {
        clientDep += w[k][0];
        for (int i = 1; i < N; i++) {
            cliDep += x[k][i][0];
        }
    }
    IloConstraint clienToDeposit = (cliDep == 0);
    IloConstraint clienToDeposit2 = (clientDep == 0);
    clienToDeposit.setName("retorno_Deposito");
    clienToDeposit2.setName("retorno_Deposito2");
    cliDep.end();
    clientDep.end();
}

void VRP::constraintTotalVehicles(){
    IloExpr sumVeiculos(env);
    for (int k = 0; k < K; k++) {
        char* namevarV;
        std::string nameV("v_" + std::to_string(k));
        namevarV = &nameV[0];
        v[k].setName(namevarV);
        sumVeiculos += v[k];
    }
    IloConstraint sumDrivers = (sumVeiculos >= 0); // total de veiculos utilizado?
    sumDrivers.setName("SumDrivers");
    model.add(sumDrivers);
    sumVeiculos.end();
}