#include "Order.hpp"

Order::Order(
    std::vector<Packet> packets_order_,
    std::vector<Vehicle> vehicles_,
    double** matrix_price_
) {
    this->packets_order = packets_order_;
    this->vehicles = vehicles_;
    this->matrix_prices = matrix_prices_;
}

void Order::createParams(){
    // Aqui tem os parametros:
    // - matrix_price, packets, vehicles, matrix custo, qi
    IloArray <IloNumArray> mp(env, K);
    for (int k = 0; k < K; k++) {
        mp[k] = IloNumArray(env, N);
    }
    IloNumArray q(env, N);								// Carga requisitado por um Packet (i)
    for (int i = 0; i < N; i++) {
        q[i] = packets_order[i].charge;
    }
    IloNumArray Qmax(env, K);								// Carga máxima do Vehicle (K)
    for (int k = 0; k < K; k++) {
        Qmax[k] = vehicles[k].charge;
    }
}

void Order::createVariables(){
    // Aqui tem as variaveis de decisoes:
    // - w_k_j : essa variavel é uma entrada para o prox model
    IloArray<IloBoolVarArray> w(env, N); // DESTINOS (index j)
    for (int i = 0; i < N; i++) {
        w[i] = IloBoolVarArray(env, K);
    }
    renameVars();
}

void Order::renameParams(){
    char* char_w;
    for(int i = 0; i < N; i++) {
        for(int k = 0; k < K; k++){
            string namew("w_" + to_string(k) + "_" + to_string(i));
            char_w = &namew[0];
            w[k][i].setName(char_w)
        }
    }
}

void Order::createFunctionObjetive(){
    // sum(mpki.wkj)
    IloExpr fo(env);
    for (int k = 0; k < K; k++) {
        for (int i = 0; i < N; i++) {
            fo += mp[i][k] * w[k][i];
        }
    }
    modelo.add(IloMinimize(env, fo));
    fo.end();
}
void Order::createConstraints(){
    constraintDestiny();
    constraintCapacitedVehicle();
}
OrderSolution Order::solve(int tempoLimite){
    // determinar um tempo, a Solution desse modelo
    // - w_k_j : sera a matrix de destinos do prox model
    IloCplex cplex(model);
    IloNum objFO = IloInfinity;
    createParams();
    createVariables();
    createFunctionObjetive();
    createConstraints();
    cplex.setParam(IloCplex::TiLim, tempo);
	cplex.extract(model);
    char* output;
    string saida("saida_" + to_string(t)+ ".lp");
    output = &saida[0];
    cplex.exportModel(output);
    IloBool result = cplex.solve();
    if(result){
        objFO = cplex.getObjValue();
        OrderSolution sol = outputOrder(cplex);
        return sol
    } else {
        OrderSolution sol = OrderSolution("Not found results factiveis");
        return sol
    }
}

void Order::constraintDestiny(){
    IloConstraintArray cons_destino(env);
    for (int j = 1; j < N; j++) {
        IloExpr restDest(env);
        char* namevar;
        string name("packetCheck_" + to_string(j));
        namevar = &name[0];
        for (int k = 0; k < K; k++) {
            restDest += w[k][j];
        }
        IloConstraint consRestDest = (restDest == 1);
        consRestDest.setName(namevar);
        modelo.add(consRestDest);
        cons_destino.add(consRestDest);
        restDest.end();
    }
}
void Order::constraintCapacitedVehicle(){
    IloConstraintArray cons_array_saida(env);
    for (int k = 0; k < K; k++) {
        IloExpr restSaida(env);
        char* namevar;
        string name("packets_ofVehicle_" + to_string(k));
        namevar = &name[0];
        for (int i = 0; i < N; i++) {
            restSaida += q[i] * w[k][i];
        }
        IloConstraint consRestSaida = (restSaida <= Qmax[k]);
        consRestSaida.setName(namevar);
        modelo.add(consRestSaida);
        cons_array_saida.add(consRestSaida);
        restSaida.end();
    }
}

OrderSolution Order::outputOrder(IloCplex cplex){
    std::string message("Sucess!");
    IloArray <IloNumArray> output(env, K);
    for(int k = 0; k < K; k++){
        output[k] = IloNumArray(env, N);
    }
    for(int k = 0; k < K; k++){
        cplex.getValues(w[k], output[k]);
    }
    OrderSolution solution = OrderSolution(output, message);
    return solution;
} 