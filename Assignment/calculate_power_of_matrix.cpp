//
//  main.cpp
//  prog7-A^k_matrix
//
//  Created by Jay Chen on 10/23/19.
//  Copyright © 2019 Jay Chen. All rights reserved.
//

#include <iostream>
#include <iomanip>  // setw(n) is in here
#include "newmat.h"
#include "newmatap.h"
#include "newmatio.h"
#include "Header.h"
#include <time.h>


int main(int argc, const char * argv[]) {
    
    matrixmulti X;
    int row,k;
    sscanf (argv[1], "%d", &k);
    sscanf (argv[2], "%d", &row);
    cout<<"Number of rows/cols of the matrix is : "<< row<< endl;
    cout<<"The expoent is : "<< k<< endl;
    X.initialize_random_matrix(row);
   
    //    cout << "Original Matrix"<<endl;
    //    cout << setw(10) << X.A << endl;
    
    clock_t start_time = clock();
    Matrix M;
    M=X.repreating_square(k);
    clock_t end_time = clock();
    cout << "Repeating Square result"<<endl;
       //    cout << setw(10) << M << endl;
    cout<<"Running time for repeating square algorithm: "<< ((float)end_time - (float) start_time)/CLOCKS_PER_SEC<<" s"<<endl;
    
    cout<<endl;

    clock_t start_time1 = clock();
    Matrix N;
    N=X.brute_force(k);
    clock_t end_time1 = clock();
    cout << "Burte Force result"<<endl;
//    cout << setw(10) << N << endl;
    cout<<"Running time for brute force algorithm: "<< ((float) end_time1 - (float) start_time1)/CLOCKS_PER_SEC<<" s"<<endl;
    
    return 0;
}
