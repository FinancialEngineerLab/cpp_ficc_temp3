//
//  main.cpp
//  prog5
//
//  Created by Jay Chen on 10/1/19.
//  Copyright © 2019 Jay Chen. All rights reserved.
//

#include <iostream>
#include <algorithm>

using namespace std;

class card_game{
public:
    int total_card;
    
    // dynamic programming
    // use a 2D array to store the value we've already calculated;
    double **store =new double*[total_card];
    // initialize the store
    void initialize(int n)
    {
        total_card=n;
        for(int i = 0; i < total_card; i++)
            store[i] = new double[total_card];
        for(int i = 0; i < n ; i++  )
            for(int j = 0; j < n ; j++)
                store[i][j] = -1;
    }
    
    double value(double r, double b)
    {
        //check if we already calculated the value(r,b).
        //if so, use the value we've stored
        //if not, calculate it
        if(store[int(r)][int(b)]!= -1)
            return store[int(r)][int(b)];
        if(r==0)
            return b;
        if(b==0)
            return 0;
        //recursivly find the next possible Value;
        double s1= (r/(r+b)) * value(r-1, b);
        double s2= (b/(r+b)) * value(r, b-1);
        // s1+s2 stands for keep playing;
        // b-r stands for stop now;
        // choose the bigger one.
    
        // store the calculated-value here.
        store[int(r)][int(b)] = max((s1+s2), (b-r));
        return max((s1+s2), (b-r));
    }
};


int main(int argc, const char * argv[]) {
    
    card_game X;
    sscanf (argv[1], "%d", &X.total_card);
    X.initialize(X.total_card);
    double r=X.total_card/2;
    double b=X.total_card/2;
    cout<<"Value of the Game: "<< X.value(r,b)<<endl;
    
    return 0;
}

