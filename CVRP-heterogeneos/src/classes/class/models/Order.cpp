#include "Order.hpp"

Order::Order(
    std::vector<Packet> packets_order_,
    std::vector<Vehicle> vehicles_,
    double** matrix_price_,
    int N_,
    int K_
) {
    this->packets_order = packets_order_;
    this->vehicles = vehicles_;
    this->matrix_prices = matrix_price_;
    this->N = N_;
    this->K = K_;
    IloModel model(env);
    this->model = model;
}

void Order::createParams(){
    // Aqui tem os parametros:
    // - matrix_price, packets, vehicles, matrix custo, qi
    IloArray <IloNumArray> mp(env, K);
    for (int k = 0; k < K; k++) {
        mp[k] = IloNumArray(env, N);
    }
    for (int k = 0; k < K; k++) {
        for (int i = 0; i < N; i++) {
            mp[k][i] = matrix_prices[k][i];
        }
    }
    IloNumArray q(env, N);								// Carga requisitado por um Packet (i)
    for (int i = 0; i < N; i++) {
        q[i] = packets_order[i].charge;
    }
    IloNumArray Qmax(env, K);								// Carga máxima do Vehicle (K)
    for (int k = 0; k < K; k++) {
        Qmax[k] = vehicles[k].charge_max;
    }
    IloNumArray e(env, K);
    for(int k = 0; k < K; k++){
        e[k] = vehicles[k].cust;
    }
    this->mp = mp;
    this->q = q;
    this->Qmax = Qmax;
    //this->e = e;
}

void Order::createVariables(){
    // Aqui tem as variaveis de decisoes:
    // - w_k_j : essa variavel é uma entrada para o prox model
    IloArray<IloBoolVarArray> w(env, K); // DESTINOS (index j)
    for (int k = 0; k < K; k++) {
        w[k] = IloBoolVarArray(env, N);
    }

    IloBoolVarArray z(env, K); // veiculo usado

    this->w = w;
    this->z = z;
    renameVars();
}

void Order::renameVars(){
    char* char_w;
    for(int i = 0; i < N; i++) {
        for(int k = 0; k < K; k++){
            std::string namew("w_" + std::to_string(k) + "_" + std::to_string(i));
            char_w = &namew[0];
            w[k][i].setName(char_w);
        }
    }

    char* char_z;
    for(int k = 0; k < K; k++){
        std::string namez("z_" + std::to_string(k));
        char_z = &namez[0];
        z[k].setName(char_z);
    }
}

void Order::createFunctionObjetive(){
    // sum(mpki.wkj)
    IloExpr fo(env);
    for (int k = 0; k < K; k++) {
        for (int i = 0; i < N; i++) {
            fo += mp[k][i] * w[k][i];
        }
        // fo += e[k]*z[k]
    }
    model.add(IloMinimize(env, fo));
    fo.end();
}

void Order::createConstraints(){
    constraintDestiny();
    constraintCapacitedVehicle();
}

Solution Order::solve(int tempoLimite){
    // determinar um tempo, a Solution desse modelo
    // - w_k_j : sera a matrix de destinos do prox model
    IloCplex cplex(model);
    IloNum objFO = IloInfinity;
    createParams();
    createVariables();
    createFunctionObjetive();
    createConstraints();
    std::cout << "Solution Order::solve2" << std::endl;
    cplex.setParam(IloCplex::TiLim, tempoLimite);
	cplex.extract(model);
    char* output;
    std::string saida("saida_order.lp");
    output = &saida[0];
    cplex.exportModel(output);
    IloBool result = cplex.solve();
    if(result){
        objFO = cplex.getObjValue();
        std::cout << "objFO: " << objFO << std::endl;
        OrderSolution out = outputOrder(cplex);
        Solution sol = Solution(out);
        return sol;
    } else {
        OrderSolution out = OrderSolution("Not found results factiveis");
        Solution sol = Solution(out);
        return sol;
    }
}

void Order::constraintDestiny(){
    IloConstraintArray cons_destino(env);
    for (int j = 1; j < N; j++) {
        IloExpr restDest(env);
        char* namevar;
        std::string name("packetCheck_" + std::to_string(j));
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
void Order::constraintCapacitedVehicle(){
    IloConstraintArray cons_array_saida(env);
    for (int k = 0; k < K; k++) {
        IloExpr restSaida(env);
        char* namevar;
        std::string name("packets_ofVehicle_" + std::to_string(k));
        namevar = &name[0];
        for (int i = 0; i < N; i++) {
            restSaida += q[i] * w[k][i];
        }
        IloConstraint consRestSaida = (restSaida <= Qmax[k]);
        consRestSaida.setName(namevar);
        model.add(consRestSaida);
        cons_array_saida.add(consRestSaida);
        restSaida.end();
    }
}

OrderSolution Order::outputOrder(IloCplex& cplex){
    std::string message("Sucess!");
    IloArray <IloNumArray> output(env, K);
    for(int k = 0; k < K; k++){
        output[k] = IloNumArray(env, N);
    }
    for(int k = 0; k < K; k++){
        cplex.getValues(w[k], output[k]);
    }
    std::cout << message << std::endl;
    OrderSolution solution = OrderSolution(output, message, packets_order, vehicles);
    return solution;
} 