\ENCODING=ISO-8859-1
\Problem name: IloCplex

Minimize
 obj1: 6089.62766302445 w_0_0 + 1170.46999107196 w_0_1 + 1140.17542509914 w_0_2
       + 1114.27106217473 w_0_3 + 1044.03065089105 w_0_4 + 1040 w_0_5
       + 1005.78327685441 w_0_6 + 1021.56742312977 w_0_7
       + 1029.5630140987 w_0_8 + 948.683298050514 w_0_9
Subject To
 packetCheck_1:       w_0_1  = 1
 packetCheck_2:       w_0_2  = 1
 packetCheck_3:       w_0_3  = 1
 packetCheck_4:       w_0_4  = 1
 packetCheck_5:       w_0_5  = 1
 packetCheck_6:       w_0_6  = 1
 packetCheck_7:       w_0_7  = 1
 packetCheck_8:       w_0_8  = 1
 packetCheck_9:       w_0_9  = 1
 packets_ofVehicle_0: 30 w_0_1 + 20 w_0_2 + 10 w_0_3 + 10 w_0_4 + 10 w_0_5
                      + 20 w_0_6 + 10 w_0_7 + 10 w_0_8 + 30 w_0_9 <= 200
Bounds
 0 <= w_0_0 <= 1
 0 <= w_0_1 <= 1
 0 <= w_0_2 <= 1
 0 <= w_0_3 <= 1
 0 <= w_0_4 <= 1
 0 <= w_0_5 <= 1
 0 <= w_0_6 <= 1
 0 <= w_0_7 <= 1
 0 <= w_0_8 <= 1
 0 <= w_0_9 <= 1
Binaries
 w_0_0  w_0_1  w_0_2  w_0_3  w_0_4  w_0_5  w_0_6  w_0_7  w_0_8  w_0_9 
End
