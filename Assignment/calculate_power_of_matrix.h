////
////  Header.h
////  prog7-A^k_matrix
////
////  Created by Jay Chen on 10/24/19.
////  Copyright © 2019 Jay Chen. All rights reserved.
////
//
#ifndef Header_h
#define Header_h

#include <iostream>
#include <cmath>
#include <random>
#include <chrono>
#include "newmat.h"
#include "newmatap.h"
#include "newmatio.h"

using namespace std;
//
//https://compass2g.illinois.edu/webapps/blackboard/execute/content/file?cmd=view&content_id=_3950519_1&course_id=_45061_1&framesetWrapped=true
//used the strassen.h/.cpp on compass2g

// use seed to generate randomness
unsigned seed = (unsigned) std::chrono::system_clock::now().time_since_epoch().count();
std::default_random_engine generator (seed);

double get_random()
{
    std::uniform_real_distribution <double> distribution(-1.0,1.0);
    double number = distribution(generator);
    return (number);
}

class matrixmulti {
    
    // construct a trivial random generator engine from a time-based seed:
    int row;
    
    //this function is used for generate the A^k, while k=2,4,8,16...
    //use repeat squaing way.
    //keep recursion,when n>1; A^k= A^k/2 * A^k/2;
    Matrix squarepow_of_A(int n)
    {
        Matrix B(row, row);
        if(n==1)
            return A;
        B = squarepow_of_A(n/2);
        return  B*B;
    }
    
public:
    Matrix A;
    void initialize_random_matrix(int r)
    {
        //initialize the random matrix with uniform_real_distribution
        row=r;
        Matrix B(row,row);
        A=B;
        for(int i=1; i<=row; i++)
            for(int j=1; j<=row; j++)
                A(i,j) = 5*get_random();
    }
    
    
    Matrix repreating_square(int k)
    {
        IdentityMatrix I(row);
        Matrix res(row,row);
        res=I;
        if(k==1) return A;
        
        //transform k to binary
        vector<int> binary_k;
        while(k>0)
        {
            binary_k.push_back(k%2);
            k =k/2;
        }
        // 3=11; the binary_k here is actually the reverse of k.
        
        //here, divide A^k into it binary multiplation.
        for(int i=0; i<binary_k.size(); i++)
        {
            if(binary_k[i] != 0)
                res=res* squarepow_of_A(pow(2, i));
        }
        return res;
    }
    
    // just recursively A^k = A * (A^k-1) * (A^k-2) ...
    
    Matrix brute_force(int k)
    {
        Matrix temp=A;
        if(k==1)
            return temp;
        return temp*brute_force(k-1);
    }
};
#endif /* Header_h */
