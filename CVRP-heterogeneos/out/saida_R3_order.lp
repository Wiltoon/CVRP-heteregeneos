\ENCODING=ISO-8859-1
\Problem name: IloCplex

Minimize
 obj1: 1950.74310623135 w_0_0 + 707.106781186548 w_0_1 + 761.577310586391 w_0_2
       + 785.87530817554 w_0_3 + 824.621125123532 w_0_4
       + 806.225774829855 w_0_5 + 647.765389628066 w_0_6
       + 670.522184569608 w_0_7 + 670.820393249937 w_0_8
       + 632.455532033676 w_0_9
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
 packets_ofVehicle_0: 10 w_0_1 + 10 w_0_2 + 20 w_0_3 + 30 w_0_4 + 20 w_0_5
                      + 20 w_0_6 + 40 w_0_7 + 20 w_0_8 + 30 w_0_9 <= 200
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
