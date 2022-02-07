#include <iostream>

#include "VRP.hpp"
#include "../Packet.hpp"
#include "../Vehicle.hpp"

VRP::VRP(
    IloArray <IloNumArray> output,
    std::vector<Packet> packets, 
    std::vector<Vehicle> vehicles
){
    this->packets = packets;
    this->vehicles = vehicles;
    this->output = output;
    this->N = packets.size();
    this->K = vehicles.size();
    IloModel model(env);
    this->model = model;
}

void VRP::createParams() {
    // nomear todas os parametros de entrada
    IloArray <IloNumArray> d(env, N); // Matrizes de distancia do pedido i->j
    for (int i = 0; i < N; i++) {
        d[i] = IloNumArray(env, N);
    }
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++){
            if(i == j){
                d[i][j] = 999999.0;
            } else {
                d[i][j] = distance_euclidian(packets[i], packets[j]);
            }
        }
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
    IloBoolArray v(env, K);
    this->d = d;
    this->p = p;
    this->e = e;
    this->v = v;
    this->w = w;
    for(int k = 0; k < K; k++){
        for (int j = 0; j < N; j++){
            this->w[k][j] = (this->output[k][j] > 0.9);
        }
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
    IloBoolVarArray y(env, N);
    IloArray<IloBoolVarArray> z(env, K); // ORIGENS (index i)
    for (int k = 0; k < K; k++) {
        z[k] = IloBoolVarArray(env, N);
    }
    IloArray<IloNumVarArray> u(env, K);
    for (int k = 0; k < K; k++) {
        u[k] = IloNumVarArray(env, N, 0, vehicles[k].charge_max);// Variavel auxiliar para eliminicao de rota
    }
    this->x = x;
    this->y = y;
    this->z = z;
    this->u = u;
    renameVars();
}

void VRP::renameVars(){
    char* char_u;
    for(int k = 0; k < K; k++){
        for(int i = 0; i < N; i++) {
            std::string nameu("u_" + std::to_string(k) + "_" + std::to_string(i));
            char_u = &nameu[0];
            u[k][i].setName(char_u);
        }
    }

    char* char_y;
    for(int i = 0; i < N; i++){
        std::string namey("y_" + std::to_string(i));
        char_y = &namey[0];
        y[i].setName(char_y);
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
    IloCplex cplex(model);
    IloNum objFO = IloInfinity;
    createParams();
    createVariables();
    createFunctionObjetive();
    createConstraints();
    VRPSolution o = relax_and_fix(timeLimite, cplex);
    Solution sol = Solution(o);
    return sol;
}

VRPSolution VRP::relax_and_fix(int time, IloCplex cplex) {
    // construção do modelo relax and fix para resolver
    IloArray <IloArray <IloExtractableArray>> relaxa = relaxAll();
    IloArray <IloArray <IloNumArray>> xSol = buildXSol();
    IloArray <IloNumArray> uSol = buildUSol();
    bool LOOPINFINITO = false;
    std::vector <int> to_visit;
    std::vector <int> visited;
    to_visit.push_back(DEPOSIT);
    for(int t = 0; t < N && pathsToFix(visited); t++){
        printerVector("TO VISIT ", to_visit);
        removeRelaxationToVisit(relaxa, to_visit);
        if(!LOOPINFINITO){
            std::cout << "t: " << std::to_string(t) << std::endl;
            IloBool result = solveIteration(t, time, cplex);
            if(result){
                assignTheSolutions(xSol,uSol,to_visit, cplex);
                fixVariables(xSol,to_visit,visited);
            } else {
                if (time < TIME_MAX){
                    std::cout << "Aumentar tempo do solve "<< time << "+"<< SOMADOR_TIME << std::endl;
                    time += SOMADOR_TIME;
                }
                LOOPINFINITO = true;
            }
        }
    }
    VRPSolution vrp = VRPSolution(
        xSol,
        uSol, 
        vehicles,
        packets
    );
    return vrp;   
}

void VRP::fixXYZ(std::vector <int> visitar, int check, int k, int i){
    x[k][visitar[check]][i].setBounds(1, 1);
    z[k][visitar[check]].setBounds(1, 1);
    y[i].setBounds(0, 0);
}


void VRP::buildNewConstraint(int entrega){
    for (int veiculo = 0; veiculo < K; veiculo++) {
        char* namevarD;
        std::string name(
            "fluxo_veiculo_" + std::to_string(veiculo) + "_da_entrega_" + std::to_string(entrega)
        );
        namevarD = &name[0];
        IloConstraint consVeiculoUnico = (
            z[veiculo][entrega] + y[entrega] == w[veiculo][entrega]
        );
        consVeiculoUnico.setName(namevarD);
        model.add(consVeiculoUnico);
    }
}

void VRP::fixedColumn(
    IloArray <IloArray <IloNumArray>> sol,
    std::vector <int> visitar,
    int check, 
    int i, int k    
){
    for (int row = 0; row < N; row++) {	//PERCORRER A COLUNA
        if (row != visitar[check]) {
            x[k][row][i].setBounds(0, 0);
            for (int kar = 0; kar < K; kar++) { // INUTILIZAR VEICULOS PARA ATENDER OS PEDIDOS DA COLUNA
                if (sol[kar][visitar[check]][i] >= 0.8) {
                    x[kar][visitar[check]][i].setBounds(1, 1);
                }
                else {
                    if (k != kar) {
                        x[kar][visitar[check]][i].setBounds(0, 0);
                    }
                }
            }
        }
    }
}

void VRP::fixedRow(
    IloArray <IloArray <IloNumArray>> sol,
    std::vector <int> visitar,
    int check, 
    int i, int k   
){
    for (int col = 0; col < N; col++) {//PERCORRENDO A LINHA
        if (col != i) {
            x[k][visitar[check]][col].setBounds(0, 0);
            for (int kar = 0; kar < K; kar++) { // INUTILIZAR VEICULOS PARA ATENDER OS PEDIDOS DA LINHA
                if (sol[kar][visitar[check]][col] >= 0.8) {
                    x[kar][visitar[check]][col].setBounds(1, 1);
                }
                else {
                    if (k != kar) {
                        x[kar][visitar[check]][col].setBounds(0, 0);
                    }
                }
            }
        }
    }
}

void VRP::fixedSimetrics(
    IloArray <IloArray <IloNumArray>> sol,
    std::vector <int> visitar,
    int check, int i
){
    for (int kar = 0; kar < K; kar++) { // INUTILIZAR VEICULOS PARA ATENDER OS PEDIDOS DOS SIMETRICOS
        x[kar][i][visitar[check]].setBounds(0, 0);
    }
}

bool VRP::findElementInVector(
        int i,
        std::vector <int> vetor
) {
    bool encontrado = false;
    for (int it = 0; it < vetor.size(); it++) {
        //cout << "VISITADO[" << it << "] = \t" << visitado[it] << endl;
        if (vetor[it] == i) {
            encontrado = true;
        }
    }
    return encontrado;
}

void VRP::calculateWhoToFix(
        IloArray <IloArray <IloNumArray>> xSol,
        std::vector <int> visitar,
        std::vector <int> visitado,
        std::vector<int> auxvisitar,
        int check, int k
){  
    for (int i = 1; i < N; i++) {
        // std::cout << "sol [" << check << "][" << i << ']' << "=" << sol[check][i] << endl;
        if (xSol[k][visitar[check]][i] >= 0.8) {
            // std::cout << "sol [" << visitar[check] << "][" << i << ']' << "=" << sol[k][visitar[check]][i] << endl;
            // FIXA VALOR DE X[i][j] ja resolvido
            fixXYZ(visitar,check,k,i);
            buildNewConstraint(i);
            fixedColumn(xSol,visitar,check,i,k);
            fixedRow(xSol,visitar,check,i,k);
            fixedSimetrics(xSol,visitar,check,i);
            if (i != 0) {
                auxvisitar.push_back(i);
            }
            if (!findElementInVector(i,visitado)) {
                if (i != 0) {
                    visitado.push_back(i);
                }
            }
        }
        // Não fazer nada caso não tenha solução concreta
    }
}

void VRP::fixVariables(
        IloArray <IloArray <IloNumArray>> xSol,
        std::vector <int> visitar,
        std::vector <int> visitado
){
    std::vector<int> auxvisitar;
    //Fixa a parte inteira da solução
    for (int check = 0; check < visitar.size(); check++) {
        for (int k = 0; k < K; k++) {
            calculateWhoToFix(xSol,visitar,visitado,auxvisitar,check,k);
        }
    }
    // Apagar a memoria do visitar e colocar o valor do auxvisitar
    visitar.clear();
    // CASO NAO TENHA MAIS NINGUEM PARA VISITAR (AUXVISITAR == 0) (O AUXVISITAR VIRA O VISITAR)
    for (int i = 0; i < auxvisitar.size(); i++) {
        //std::cout << "AUXVISITAR[" << i << "] = \t" << auxvisitar[i] << std::endl;
        if (auxvisitar[i] != 0) {
            visitar.push_back(auxvisitar[i]);
        }
    }
}

void VRP::assignTheSolutions(
    IloArray <IloArray <IloNumArray>> xSol,
    IloArray <IloNumArray> uSol,
    std::vector <int> visitar,
    IloCplex cplex
){
    for (int k = 0; k < K; k++) {
        cplex.getValues(u[k], uSol[k]);
        for (int i = 0; i < visitar.size(); i++) {
            cplex.getValues(x[k][visitar[i]], xSol[k][visitar[i]]);
        }
    }
}

IloBool VRP::solveIteration(
    int iteration,
    int tempo,
    IloCplex cplex
){
    cplex.setParam(IloCplex::TiLim, tempo);
    cplex.extract(model);
    char* outputer;
    std::string saida("saida_" + std::to_string(iteration)+ ".lp");
    outputer = &saida[0];
    cplex.exportModel(outputer);
    
    return cplex.solve();
}

void VRP::removeRelaxationToVisit(
        IloArray <IloArray<IloExtractableArray>> relaxa,
        std::vector<int> visitar
){
    for (int k = 0; k < K; k++) {
        for (int i = 0; i < visitar.size(); i++) {
            for (int j = 0; j < N; j++) {
                model.remove(relaxa[k][visitar[i]][j]);
            }
        }
    }
}

bool VRP::pathsToFix(std::vector<int> visited){
    int packets_exclude_deposit = packets.size()-1;
    return (packets_exclude_deposit > visited.size());
}

void VRP::printerVector(std::string name, std::vector<int> elements){
    std::cout << name << " -> " << std::endl << "\t[";
    for(int i = 0; i < elements.size(); i++){
        if(i == elements.size()-1){
            std::cout << elements[i] << "]" << std::endl;
        } else {
            std::cout << elements[i] << ", ";
        }
    }
}

IloArray <IloNumArray> VRP::buildUSol(){
    IloArray <IloNumArray> uSol(env, K);
    for (int k = 0; k < K; k++) {
        uSol[k] = IloNumArray(env, N);
    }
    return uSol;
}

IloArray <IloArray <IloNumArray>> VRP::buildXSol(){
    IloArray <IloArray <IloNumArray>> xSol(env, K);
    for (int k = 0; k < K; k++) {
        xSol[k] = IloArray <IloNumArray>(env, N);
        for (int i = 0; i < N; i++) {
            xSol[k][i] = IloNumArray(env, N);
        }
    }
    return xSol;
}

IloArray <IloArray<IloExtractableArray>> VRP::relaxAll(){
    IloArray <IloArray<IloExtractableArray>> relaxa(env, K);
    for (int k = 0; k < K; k++) {
        relaxa[k] = IloArray<IloExtractableArray>(env, N);
        for (int i = 0; i < N; i++) {
            relaxa[k][i] = IloExtractableArray(env, N);
        }
    }
    for (int k = 0; k < K; k++) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (i == j) {
                    x[k][i][j].setBounds(0, 0);
                }
                else {
                    x[k][i][j].setBounds(0, 1);
                }
                relaxa[k][i][j] = IloConversion(env, x[k][i][j], ILOFLOAT);
                model.add(relaxa[k][i][j]);
            }
        }
    }
    return relaxa;
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
        sumVeiculos += v[k];
    }
    IloConstraint sumDrivers = (sumVeiculos >= 0); // total de veiculos utilizado?
    sumDrivers.setName("SumDrivers");
    model.add(sumDrivers);
    sumVeiculos.end();
}

double VRP::distance_euclidian(Packet origin, Packet destiny){
    double dx = (double)destiny.loc_x - (double)origin.loc_x;
    double dy = (double)destiny.loc_y - (double)origin.loc_y;
    return sqrt(dx * dx + dy * dy);
}