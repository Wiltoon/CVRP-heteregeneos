\ENCODING=ISO-8859-1
\Problem name: IloCplex

Minimize
 obj1: 2354.94983158242 w_0_0 + 761.577310586391 w_0_1 + 787.146746166177 w_0_2
       + 806.225774829855 w_0_3
Subject To
 packetCheck_1:       w_0_1  = 1
 packetCheck_2:       w_0_2  = 1
 packetCheck_3:       w_0_3  = 1
 packets_ofVehicle_0: 20 w_0_1 + 10 w_0_2 + 40 w_0_3 <= 200
Bounds
 0 <= w_0_0 <= 1
 0 <= w_0_1 <= 1
 0 <= w_0_2 <= 1
 0 <= w_0_3 <= 1
Binaries
 w_0_0  w_0_1  w_0_2  w_0_3 
End
