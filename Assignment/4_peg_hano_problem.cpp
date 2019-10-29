//
//  main.cpp
//  4peg-hanio
//
//  Created by Jay Chen on 10/21/19.
//  Copyright © 2019 Jay Chen. All rights reserved.
//

#include <iostream>
#include <cmath>
using namespace std;
void Move_Using_Four_Pegs (int n, char source, char destination, char intermediate_1, char intermediate_2)
{
    if (n > 0) {
        int k = floor(sqrt(2*n)); //This is the optimal value for k
        Move_Using_Four_Pegs (n-k, source, intermediate_1, intermediate_2, destination);
        Move_Using_Four_Pegs (k-1, source, intermediate_2, destination, intermediate_1);
        cout<<"Move the top disk from peg "<< source<< " to peg "<< destination<<endl;
        Move_Using_Four_Pegs (k-1, intermediate_2, destination, intermediate_1, source);
        Move_Using_Four_Pegs (n-k, intermediate_1, destination, intermediate_2, source);
    }
    /* if n == 0, do nothing */
    return;
}

int main(int argc, const char * argv[]) {
    // insert code here...
    int no_of_disks = 4;
    //cout << "Enter the #disks in peg A: ";
    //cin >> no_of_disks;
    
    if (no_of_disks <= 10)
        Move_Using_Four_Pegs (no_of_disks, 'A', 'D', 'B', 'C');
    else {
        cout << "This is going to take too long... exiting" << endl;
        exit (0);
    }
    return 0;
}
