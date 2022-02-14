\ENCODING=ISO-8859-1
\Problem name: IloCplex

Minimize
 obj1: 2924.50250840484 w_0_0 + 707.106781186548 w_0_1 + 647.765389628066 w_0_2
       + 620.966987850401 w_0_3 + 594.64274989274 w_0_4 + 538.51648071345 w_0_5
       + 487.031826475437 w_0_6 + 529.528091794949 w_0_7
       + 509.901951359278 w_0_8 + 412.310562561766 w_0_9
       + 447.213595499958 w_0_10
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
 packetCheck_10:      w_0_10  = 1
 packets_ofVehicle_0: 20 w_0_1 + 10 w_0_2 + 20 w_0_3 + 30 w_0_4 + 30 w_0_5
                      + 10 w_0_6 + 10 w_0_7 + 20 w_0_8 + 10 w_0_9 + 10 w_0_10
                      <= 200
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
 0 <= w_0_10 <= 1
Binaries
 w_0_0  w_0_1  w_0_2  w_0_3  w_0_4  w_0_5  w_0_6  w_0_7  w_0_8  w_0_9  w_0_10 
End
