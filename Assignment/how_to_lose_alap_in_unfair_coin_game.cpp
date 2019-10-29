//
//  main.cpp
//  prog6-tosscoin
//
//  Created by Jay Chen on 10/18/19.
//  Copyright © 2019 Jay Chen. All rights reserved.
//

// IE523: Financial Computation
// "How to lose as little as possible" by Addona, Wagon and Wilf
// Written by Prof. Sreenivas
//

#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include <time.h>
#include "hint.h"
using namespace std;
    
int main (int argc, char* argv[])
{
    I_have_nothing_apropos_for_this_class x;
    double alice_success_prob, bob_success_prob;
    sscanf (argv[1], "%lf", &alice_success_prob);
    sscanf (argv[2], "%lf", &bob_success_prob);
    
    cout << "Probability of success for Alice = " << alice_success_prob << endl;
    cout << "Probability of success for Bob = " << bob_success_prob << endl;
   
    clock_t start_time = clock();
    
    x.set_probability(alice_success_prob, bob_success_prob);
    // was used for test 
//    cout<<x.simulated_value(24, 1000000)<<"****";
//    cout<<x.theoretical_value(0.18, 0.2, 24)<<endl;
//    cout<<x.simulated_value(25, 1000000)<<"****";
//    cout<<x.theoretical_value(0.18, 0.2, 25)<<endl;
//    cout<<x.simulated_value(26, 1000000)<<"****";
//    cout<<x.theoretical_value(0.18, 0.2, 26)<<endl;
//    cout<<x.simulated_value(27, 1000000)<<"****";
//    cout<<x.theoretical_value(0.18, 0.2, 27)<<endl;
//    cout<<x.simulated_value(28, 1000000)<<"****";
//    cout<<x.theoretical_value(0.18, 0.2, 28)<<endl;
    int optimal = x.search_result();
    if (optimal > 0)
        cout << "The optimal number of coin tosses in each game is " << optimal << endl;
    else {
        cout << "The optimal number of coin tosses in each game exceeds 100... Quitting" << endl;
    }
    
    clock_t end_time = clock();
    cout<<"Running time: "<< (end_time-start_time)/CLOCKS_PER_SEC*1000<<" ms"<<endl;
}
    
    
    
