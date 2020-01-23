//
//  main.cpp
//  prog9-option pricing by memorization
//
//  Created by Jay Chen on 11/14/19.
//  Copyright © 2019 Jay Chen. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include <cmath>
#include <fstream>
#include <cstdlib>
#include <algorithm>
#include <vector>

using namespace std;

double up_factor, uptick_prob, notick_prob, downtick_prob, risk_free_rate, strike_price;
double initial_stock_price, expiration_time, volatility, R;
int no_of_divisions;

vector<vector<double>> callstore;
vector<vector<double>> putstore;

double american_call_option(int k, int i, double current_stock_price)
{
    if(callstore[k][no_of_divisions+i] != -1.0)
        return callstore[k][no_of_divisions+i];
        
    if (k == no_of_divisions)
    {
        callstore[k][no_of_divisions+i] = max(0.0, (current_stock_price - strike_price));
        return callstore[k][no_of_divisions+i];
    }
    else
    {
        callstore[k][no_of_divisions+i] = max((current_stock_price - strike_price),
        (uptick_prob*american_call_option(k+1, i+1, current_stock_price*up_factor) +
         notick_prob*american_call_option(k+1, i, current_stock_price)
         +downtick_prob*american_call_option(k+1, i-1, current_stock_price/up_factor))/R);
        return callstore[k][no_of_divisions+i];
    }
}

double american_put_option(int k, int i, double current_stock_price)
{
    if(putstore[k][no_of_divisions+i] != -1.0)
        return putstore[k][no_of_divisions+i];
    
    if (k == no_of_divisions)
    {
        putstore[k][no_of_divisions+i]=max(0.0, (strike_price - current_stock_price));
        return putstore[k][no_of_divisions+i];
    }
    else
    {
        putstore[k][no_of_divisions+i] = max((strike_price - current_stock_price),
        (uptick_prob*american_put_option(k+1, i+1, current_stock_price*up_factor) + notick_prob*american_put_option(k+1, i, current_stock_price) +
         downtick_prob*american_put_option(k+1, i-1, current_stock_price/up_factor))/R);
        return putstore[k][no_of_divisions+i];
    }
}

int main (int argc, char* argv[])
{
    
    sscanf (argv[1], "%lf", &expiration_time);
    sscanf (argv[2], "%d", &no_of_divisions);
    sscanf (argv[3], "%lf", &risk_free_rate);
    sscanf (argv[4], "%lf", &volatility);
    sscanf (argv[5], "%lf", &initial_stock_price);
    sscanf (argv[6], "%lf", &strike_price);
    
    vector<double> tmp;
    tmp.resize(2*no_of_divisions+2, -1.0);
    callstore.resize(2*no_of_divisions+2, tmp);
    putstore.resize(2*no_of_divisions+2, tmp);
    
    up_factor = exp(volatility*sqrt((2*expiration_time)/((float) no_of_divisions)));
    R = exp(risk_free_rate*expiration_time/((float) no_of_divisions));
    uptick_prob = pow((sqrt(R) - 1/sqrt(up_factor))/(sqrt(up_factor)-1/sqrt(up_factor)),2);
    downtick_prob = pow((sqrt(up_factor) - sqrt(R))/(sqrt(up_factor)-1/sqrt(up_factor)),2);
    notick_prob = 1 - uptick_prob - downtick_prob;
    
    cout << "Recursive Trinomial American-Asian Option Pricing by Memorization" << endl;
    cout << "Expiration Time (Years) = " << expiration_time << endl;
    cout << "Number of Divisions = " << no_of_divisions << endl;
    cout << "Risk Free Interest Rate = " << risk_free_rate << endl;
    cout << "Volatility (%age of stock value) = " << volatility*100 << endl;
    cout << "Initial Stock Price = " << initial_stock_price << endl;
    cout << "Strike Price = " << strike_price << endl;
    
    cout << "--------------------------------------" << endl;
    cout << "Up Factor = " << up_factor << endl;
    cout << "Uptick Probability = " << uptick_prob << endl;
    cout << "Downtick Probability = " << downtick_prob << endl;
    cout << "Notick Probability = " << notick_prob << endl;
    
    cout << "--------------------------------------" << endl;
    double call_price = american_call_option(0, 0,initial_stock_price);
    cout << "Trinomial Price of an American Call Option = " << call_price << endl;
    double put_price = american_put_option(0, 0, initial_stock_price);
    cout << "Trinomial Price of an American Put Option = " << put_price << endl;
    cout << "--------------------------------------" << endl;
    cout << "Let us verify the Put-Call Parity: S+P-C = Kexp(-r*T) for American Options" << endl;
    cout <<  initial_stock_price << " + " << put_price << " - " << call_price;
    cout << " = " << strike_price << "exp(-" << risk_free_rate << " * " << expiration_time << ")" << endl;
    cout << initial_stock_price + put_price - call_price << " ?=? " << strike_price*exp(-risk_free_rate*expiration_time) << endl;
    if (abs(initial_stock_price + put_price - call_price - strike_price*exp(-risk_free_rate*expiration_time)) <= 1e-3)
        cout << "Looks like Put-Call Parity holds within three decimal places" << endl;
    else
        cout << "Looks like Put-Call Parity does NOT hold" << endl;
    cout << "--------------------------------------" << endl;

}

