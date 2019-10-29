//
//  prog6-tosscoin
//
//  Created by Jay Chen on 10/18/19.
//  Copyright © 2019 Jay Chen. All rights reserved.
//

#ifndef ALICE_AND_BOB
#define ALICE_AND_BOB

#include <cmath>

#define NUMBER_OF_TRAILS 500000
#define N 30
using namespace std;

class I_have_nothing_apropos_for_this_class
{
private:
    double alice_probability, bob_probability;
    
    // private member function: uniform RV generator
    double get_uniform()
    {
        // write the appropriate code here
        return (((float) random())/(pow(2.0, 31.0)-1.0));
    }
    
    // private member function: nCi (i.e. n-take-i)
    int take(int n, int i)
    {
        // write a **RECURSIVE** implementation of n-take-i.
        // If you made it non-recurisive (i.e. n!/((n-i)!i!)) -- it
        // will take too long for large sizes
        
        //recursion end condition.
        if( n==i || i==0 )
            return 1;
        
        // use the recursive relationship found by Blaise Pascal,
        //nCi = (n-1)C(i-1) + (n-1)Ci;
        // also in notes in Lesson1: Recursion
        int t1 = take(n-1, i-1);
        int t2 = take(n-1, i);
        return t1+t2;
    }
    
    // this routine implements the probability that Alice has more
    // heads than Bob after n-many coin tosses
    double theoretical_value(double q, double p, int n)
    {
        // implement equation 1.1 of Addona-Wagon-Wilf paper
        
        //according to Equation 1.1,
        //P(Alice wins) = cigma P(Bob got r heads) * P(Alice got s>r heads)
        //fn = cigma(r>=0) (nCr * p^r * (1-p)^n-r) * Alice_got_more_heads
        //Alice_got_more_heads = cigma(s>r) nCs * q^s * (1-q)^n-s
        
        double probability_Alice_wins=0;
        //in this loop. if r==n, there is no way Alice could win. Cuz Alice couldn't get more heads.
        for(int r=0; r<=n; r++)
        {
            double probability_Alice_got_more_heads=0;
            if(r==n)
                probability_Alice_got_more_heads=0;
            else{
                for(int s=r+1; s<=n ; s++)
                    probability_Alice_got_more_heads += take(n,s) * pow(q,s) * pow((1-q),(n-s));
            }
            probability_Alice_wins += take(n,r) * pow(p, r) * pow((1-p), (n-r)) * (probability_Alice_got_more_heads);
        }
        return probability_Alice_wins;
    }

public:
    // public function:
    
    //use 2 array to store the output
    double probility_of_Alice_win_simulate[N];
    double probility_of_Alice_win_theoretical[N];
    
    void set_probability(double alice_p, double bob_p)
    {
        alice_probability = alice_p;
        bob_probability = bob_p;
    }
    
    // probability of Alice winning the game.
    double simulated_value(int number_of_coin_tosses_in_each_game, int no_of_trials)
    {
        int no_of_wins_for_alice = 0;
        for (int i = 0; i < no_of_trials; i++)
        {
            int number_of_heads_for_alice = 0;
            int number_of_heads_for_bob = 0;
            for (int j = 0; j < number_of_coin_tosses_in_each_game; j++)
            {
                if (get_uniform() < alice_probability)
                    number_of_heads_for_alice++;
                if (get_uniform() < bob_probability)
                    number_of_heads_for_bob++;
            }
            if (number_of_heads_for_alice > number_of_heads_for_bob)
                no_of_wins_for_alice++;
        }
        return ( ((double) no_of_wins_for_alice)/((double) no_of_trials) );
    }
        
    int search_result()
    {
        // implememt a discrete-search procedure for the optimal n-value.
        // start with n = 1 and find the discrete-value of n that has
        // the largest probability for Alice winning.  Why would this work?
        // See Theorem 2.2 of the paper for the reason!
        
        
        int best_try = -1;
        double max_Alice_win_probability=0;
        
        //toss i coins, each toss NUMBER_OF_TRAILS times.
        //in order to find the biggest probability, we need to flag one first, when we find one bigger than the old max one, then we replace it.
        for(int i=1; i<=N; i++)
        {
            cout<<"toss "<<i<<" coins"<<" ";
            
            double temp;
            temp = simulated_value(i, NUMBER_OF_TRAILS);
            
            probility_of_Alice_win_simulate[i-1]=temp;
            cout<<"simulate probability: "<< temp<<" ";
    
            
            if(temp > max_Alice_win_probability)
            {
                max_Alice_win_probability = temp;
                best_try = i;
            }
            
            //just calculate the theoretical value here.
            probility_of_Alice_win_theoretical[i-1] = theoretical_value(alice_probability, bob_probability, i);
            cout<<"theoretical probability: "<<probility_of_Alice_win_theoretical[i-1]<<endl;
            
        }
        return best_try;
    }
    
};
#endif









