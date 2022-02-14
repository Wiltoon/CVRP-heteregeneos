\ENCODING=ISO-8859-1
\Problem name: IloCplex

Minimize
 obj1: 1989.25668543039 w_0_0 + 616.116872029975 w_0_1 + 721.110255092798 w_0_2
       + 666.033032213869 w_0_3 + 707.106781186548 w_0_4
       + 781.024967590665 w_0_5
Subject To
 packetCheck_1:       w_0_1  = 1
 packetCheck_2:       w_0_2  = 1
 packetCheck_3:       w_0_3  = 1
 packetCheck_4:       w_0_4  = 1
 packetCheck_5:       w_0_5  = 1
 packets_ofVehicle_0: 30 w_0_1 + 40 w_0_2 + 20 w_0_3 + 20 w_0_4 + 10 w_0_5
                      <= 200
Bounds
 0 <= w_0_0 <= 1
 0 <= w_0_1 <= 1
 0 <= w_0_2 <= 1
 0 <= w_0_3 <= 1
 0 <= w_0_4 <= 1
 0 <= w_0_5 <= 1
Binaries
 w_0_0  w_0_1  w_0_2  w_0_3  w_0_4  w_0_5 
End
