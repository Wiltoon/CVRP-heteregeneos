\ENCODING=ISO-8859-1
\Problem name: IloCplex

Minimize
 obj1: 1207.27128175564 w_0_0 + 260 w_0_1 + 200 w_0_2 + 203.960780543711 w_0_3
       + 243.310501211929 w_0_4 + 300 w_0_5 + 302.654919008431 w_0_6
       + 316.227766016838 w_0_7 + 342.344855372474 w_0_8
       + 354.400902933387 w_0_9 + 400 w_0_10 + 412.310562561766 w_0_11
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
 packetCheck_11:      w_0_11  = 1
 packets_ofVehicle_0: 10 w_0_1 + 10 w_0_2 + 20 w_0_3 + 20 w_0_4 + 10 w_0_5
                      + 40 w_0_6 + 10 w_0_7 + 10 w_0_8 + 20 w_0_9 + 10 w_0_10
                      + 10 w_0_11 <= 200
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
 0 <= w_0_11 <= 1
Binaries
 w_0_0  w_0_1  w_0_2  w_0_3  w_0_4  w_0_5  w_0_6  w_0_7  w_0_8  w_0_9  w_0_10 
 w_0_11 
End
