\ENCODING=ISO-8859-1
\Problem name: IloCplex

Minimize
 obj1: 3513.66784046863 w_0_0 + 761.577310586391 w_0_1 + 806.225774829855 w_0_2
       + 670.820393249937 w_0_3 + 884.081444212014 w_0_4
       + 860.232526704263 w_0_5 + 812.157620169878 w_0_6
       + 744.043009509531 w_0_7 + 721.110255092798 w_0_8
       + 616.116872029975 w_0_9
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
 packets_ofVehicle_0: 20 w_0_1 + 30 w_0_2 + 10 w_0_3 + 20 w_0_4 + 40 w_0_5
                      + 10 w_0_6 + 30 w_0_7 + 10 w_0_8 + 20 w_0_9 <= 200
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
