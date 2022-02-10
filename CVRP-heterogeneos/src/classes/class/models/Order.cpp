#include "Order.hpp"

Order::Order(
    std::vector<Packet> packets_order_,
    std::vector<Vehicle> vehicles_,
    double alpha, int region
) {  
    this->packets_order = packets_order_;
    this->vehicles = vehicles_;
    this->N = packets_order_.size();
    this->K = vehicles_.size();
    this->region = region;
    this->matrix_prices = calculate_matrix_price(alpha, N, K);
    IloModel model(env);
    this->model = model;
}

double** Order::calculate_matrix_price(double alpha, int N, int K){
    double** matrixPrice;
    //matrix de preço para cada veiculo
    //O preço a se pagar para entregar o packet pelo veiculo Kmatrix_price = (double**) malloc(K * sizeof(double));
    matrixPrice = (double**) malloc(K * sizeof(double*));
    for (int k = 0; k < K; k++){
        matrixPrice[k] = (double*)malloc(N * sizeof(double));
    }
    // std::cout << "K, N = " << K << "\t" << N << std::endl;
    for(int k = 0; k < K; k++){
        for(int i = 0; i < N; i++){
            matrixPrice[k][i] = price_packet_per_vehicle(
                this->packets_order[i], this->vehicles[k], alpha
            );
            // std::cout << "WHERE  k:" << k << std::endl;
            // std::cout << "matrixPrice[" << k << "][" << i << "] => " << matrixPrice[k][i] << std::endl; 
            // std::cout << "[k][i] => " << k << "\t" << i << std::endl;  
        }
    }
    
    return matrixPrice;
}

bool Order::crescentePackets(const NeighborPacket& p1, const NeighborPacket& p2){
    return (p1.distance < p2.distance);
}

std::vector<NeighborPacket> Order::sortPacketsAroundPacket(Packet pac){
    // ordenar dos pacotes mais proximos do pacote "pac"
    // armazenar em um vetor em ordem crescente as distancias
    int N = packets_order.size();
    std::vector<NeighborPacket> pkts_bor;
    //somente os pedidos devem aparecer aqui
    for(int i = 0; i < N; i++){
        if(packets_order[i].id != 0){
            NeighborPacket nbp = NeighborPacket(pac,packets_order[i]);
            pkts_bor.push_back(nbp);
        }
    }
    std::sort(pkts_bor.begin(), pkts_bor.end(), crescentePackets);
    return pkts_bor;
}

double Order::distance_euclidian(Packet origin, Packet destiny){
    double dx = (double)destiny.loc_x - (double)origin.loc_x;
    double dy = (double)destiny.loc_y - (double)origin.loc_y;
    return sqrt(dx * dx + dy * dy);
}

double Order::price_packet_per_vehicle(
    Packet& packet, Vehicle& vehicle, double alpha
){
    // distancia a percorrer do deposito ate ele, e os K mais proximos
    // ordenar os pacotes mais proximos do packet
    std::vector<NeighborPacket> sorted = sortPacketsAroundPacket(packet);
    double CAP_PROBABILITY = alpha * vehicle.charge_max;
    double price_dist = 0.0;
    for(Packet& p : packets_order){
        if(p.id == 0){
            price_dist = distance_euclidian(p, packet);
        }
    }
    vehicle.addPacket(packet);
    int cap = vehicle.current_charge;
    for(NeighborPacket nbp : sorted){
        cap += nbp.getDestination().charge;
        if(cap < CAP_PROBABILITY){
            vehicle.addPacket(nbp.getDestination());
            price_dist += distance_euclidian(
                nbp.getOrigin(), nbp.getDestination());
        } 
    }  
    return (price_dist * vehicle.cust);
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
    cplex.setOut(env.getNullStream());
	cplex.extract(model);
    char* output;
    std::string saida("out/saida_R" + std::to_string(region) + "_order.lp");
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
        OrderSolution out = OrderSolution("NOT Results feasible Order Solution");
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
    std::string message("Packets Per Vehicle Sucess!");
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