#include <iostream>

#include "CCP.hpp"

CCP::CCP(
        std::vector<Packet> packets_,
        std::vector<Vehicle> vehicles_used_,
        double** matrix
    ){
        this->deliveries = packets_;
        this->vehicles = vehicles_used_;
        this->matrix_distance = matrix;
        this->N = packets_.size();
        this->K = vehicles_used_.size();
        IloModel model(env);
        this->model = model;
}

void CCP::createParams(){
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
                d[i][j] = matrix_distance[i][j];
            }
        }
    }
    // Carga dos Pacotes
    IloNumArray p(env, N);
    for(int i=0; i<N; i++){
        p[i] = deliveries[i].charge;
    }
    // Custo dos veículos
    IloNumArray c(env, K);
    // Carga maxima do veículo k
    IloNumArray Qr(env, K);
    for (int k = 0; k < K; k++) {
        c[k] = vehicles[k].cust;
        Qr[k] = vehicles[k].charge_max;
    }
    this->d = d;
    this->p = p;
    this->c = c;
    this->Qr = Qr;
}

void CCP::createVariables(){
    // Pacote atendido pelo veiculo k w_kj
    IloArray<IloBoolVarArray> w(env, K); // DESTINOS (index j)
    for (int k = 0; k < K; k++){
        w[k] = IloBoolVarArray(env, N);
    }
    this->w = w;
    renameVars();
}

void CCP::renameVars(){
    char* char_w;
    for(int k = 0; k < K; k++){
        for(int i = 0; i < N; i++) {
            std::string namew("w_" + std::to_string(k) + "_" + std::to_string(i));
            char_w = &namew[0];
            w[k][i].setName(char_w);
        }
    }
}

void CCP::createFunctionObjetive() {
    // criar a funcao objetivo no ambiente env e_k pode ser custo por km
    IloExpr fo(env);
    for (int k = 0; k < K; k++) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (i != j){
                    fo += d[i][j] * w[k][i] * w[k][j] * c[k];
                }
            }
        }
    }
    model.add(IloMinimize(env, fo));
    fo.end();
}

void CCP::createConstraints(){
    constraintSeparatedPackets();
    constraintMaxCapacited();
    constraintIncludedDeposit();
}

Solution CCP::solve(int timeLimite) {
    // aqui deve resolver o problema VRP
    IloCplex cplex(model);
    IloNum objFO = IloInfinity;
    createParams();
    createVariables();
    createFunctionObjetive();
    createConstraints();
    Solution sol = mip(timeLimite, cplex);
    // Solution sol = Solution(o);
    return sol;
}

Solution CCP::mip(int timeLimite, IloCplex & cplex){
    IloArray <IloNumArray> wSol = buildWSol();
    IloBool result = solveMIP(timeLimite, cplex);
    if(result){
        for (int k = 0; k < K; k++){
            cplex.getValues(w[k], wSol[k]);
        }
        OrderSolution out = outputOrder(cplex);
        Solution sol = Solution(out);
        return sol;
    } else {
        OrderSolution out = OrderSolution(
            "NOT Results feasible Order Solution");
        Solution sol = Solution(out);
        return sol;
    }

}

OrderSolution CCP::outputOrder(IloCplex& cplex) {
    std::string message("CCP Sucess!");
    IloArray <IloNumArray> output(env, K);
    for (int k = 0; k < K; k++) {
        output[k] = IloNumArray(env, N);
    }
    for (int k = 0; k < K; k++) {
        cplex.getValues(w[k], output[k]);
    }
    // std::cout << message << std::endl;
    OrderSolution solution = OrderSolution(output, message, deliveries, vehicles);
    return solution;
}

IloBool CCP::solveMIP(int timeLimite, IloCplex & cplex){
    cplex.setParam(IloCplex::TiLim, timeLimite);
    cplex.extract(model);
    char* outputer;
    std::string saida("out/managerPack.lp");
    outputer = &saida[0];
    cplex.setOut(env.getNullStream());
    cplex.exportModel(outputer);
    return cplex.solve();
}

void CCP::constraintSeparatedPackets(){
    IloConstraintArray cons_sep(env);
    for(int i = 1; i < N; i++){
        IloExpr sep(env);
        char* namevar;
        std::string name(
            "vehiclPoint_" + std::to_string(i)
        );
        namevar = &name[0];
        for (int k = 0; k < K; k++){
            sep += w[k][i];
        }
        IloConstraint consSep = (sep == 1);
        consSep.setName(namevar);
        model.add(consSep);
        cons_sep.add(consSep);
        sep.end();
    }
}
void CCP::constraintMaxCapacited(){
    IloConstraintArray cons_reg(env);
    for (int k = 0; k < K; k++){
        IloExpr restCap(env);
        char* namevar;
        std::string name("vehicle_"+std::to_string(k));
        namevar = &name[0];
        for (int i = 0; i < N; i++){
            restCap += p[i] * w[k][i];
        }
        IloConstraint conCap = (restCap <= Qr[k]);
        conCap.setName(namevar);
        model.add(conCap);
        cons_reg.add(conCap);
        restCap.end();
    }
}
void CCP::constraintIncludedDeposit(){
    IloConstraintArray cons_dep(env);
    for (int k = 0; k < K; k++){
        char* namevar;
        std::string name("vehicle_"+std::to_string(k));
        namevar = &name[0];
        IloConstraint conDep = (w[k][0] == 1);
        conDep.setName(namevar);
        model.add(conDep);
        cons_dep.add(conDep);
    }
}

IloArray <IloNumArray> CCP::buildWSol(){
    IloArray <IloNumArray> wSol(env, K);
    for (int k = 0; k < K; k++) {
        wSol[k] = IloNumArray(env, N);
    }
    return wSol;
}