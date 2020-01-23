//
//  main.cpp
//  prog9-eur option pricing by memorization
//
//  Created by Jay Chen on 11/14/19.
//  Copyright © 2019 Jay Chen. All rights reserved.
//
#include <iostream>
#include <iomanip>
#include <cmath>
#include <fstream>
#include <cstdlib>
#include <vector>


using namespace std;

double up_factor, uptick_prob, notick_prob, downtick_prob, risk_free_rate, strike_price;
double initial_stock_price, expiration_time, volatility, R;
int no_of_divisions;

vector<vector<double>> callstore;
vector<vector<double>> putstore;

// BS model comes from https://compass2g.illinois.edu/webapps/blackboard/execute/content/file?cmd=view&content_id=_3951352_1&course_id=_45061_1&framesetWrapped=true

double max(double a, double b) {
    return (b < a )? a:b;
}

double N(const double& z) {
    if (z > 6.0) { return 1.0; }; // this guards against overflow
    if (z < -6.0) { return 0.0; };
    double b1 = 0.31938153;
    double b2 = -0.356563782;
    double b3 = 1.781477937;
    double b4 = -1.821255978;
    double b5 = 1.330274429;
    double p = 0.2316419;
    double c2 = 0.3989423;
    double a=fabs(z);
    double t = 1.0/(1.0+a*p);
    double b = c2*exp((-z)*(z/2.0));
    double n = ((((b5*t+b4)*t+b3)*t+b2)*t+b1)*t;
    n = 1.0-b*n;
    if ( z < 0.0 ) n = 1.0 - n;
    return n;
};

double option_price_put_black_scholes(const double& S,      // spot price
                                      const double& K,      // Strike (exercise) price,
                                      const double& r,      // interest rate
                                      const double& sigma,  // volatility
                                      const double& time){
    double time_sqrt = sqrt(time);
    double d1 = (log(S/K)+r*time)/(sigma*time_sqrt) + 0.5*sigma*time_sqrt;
    double d2 = d1-(sigma*time_sqrt);
    return K*exp(-r*time)*N(-d2) - S*N(-d1);
};

double option_price_call_black_scholes(const double& S,       // spot (underlying) price
                                       const double& K,       // strike (exercise) price,
                                       const double& r,       // interest rate
                                       const double& sigma,   // volatility
                                       const double& time) {  // time to maturity
    double time_sqrt = sqrt(time);
    double d1 = (log(S/K)+r*time)/(sigma*time_sqrt)+0.5*sigma*time_sqrt;
    double d2 = d1-(sigma*time_sqrt);
    return S*N(d1) - K*exp(-r*time)*N(d2);
};


double european_call_option(int k, int i)
{
    if(callstore[k][no_of_divisions+i] != -1.0)
        return callstore[k][no_of_divisions+i];
    
    if (k == no_of_divisions)
    {
        callstore[k][no_of_divisions+i] = max(0.0, (initial_stock_price*pow(up_factor, ((double) i))) - strike_price);
        return callstore[k][no_of_divisions+i];
    }
    else
    {
        callstore[k][no_of_divisions+i] = (uptick_prob*european_call_option(k+1, i+1) + notick_prob*european_call_option(k+1, i) + downtick_prob*european_call_option(k+1, i-1))/R;
        return callstore[k][no_of_divisions+i];
    }
}

double european_put_option(int k, int i) {
    if(putstore[k][no_of_divisions+i] != -1.0)
        return putstore[k][no_of_divisions+i];
    
    if (k == no_of_divisions)
    {
        putstore[k][no_of_divisions+i] = max(0.0, strike_price - (initial_stock_price*pow(up_factor, ((double) i))));
        return putstore[k][no_of_divisions+i];
    }
    else
    {
        putstore[k][no_of_divisions+i] =
        (uptick_prob*european_put_option(k+1, i+1) + notick_prob*european_put_option(k+1, i) +
         downtick_prob*european_put_option(k+1, i-1))/R;
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
    
    cout << "Recursive Trinomial European Option Pricing by Memorization" << endl;
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
    double call_price = european_call_option(0, 0);
    cout << "Trinomial Price of an European Call Option = " << call_price << endl;
    cout << "Call Price according to Black-Scholes = " <<
    option_price_call_black_scholes(initial_stock_price, strike_price, risk_free_rate,
                                    volatility, expiration_time) << endl;
    
    cout << "--------------------------------------" << endl;
    double put_price = european_put_option(0, 0);
    cout << "Trinomial Price of an European Put Option = " << put_price << endl;
    cout << "Put Price according to Black-Scholes = " <<
    option_price_put_black_scholes(initial_stock_price, strike_price, risk_free_rate,
                                   volatility, expiration_time) << endl;
    
    cout << "--------------------------------------" << endl;
    cout << "Verifying Put-Call Parity: S+P-C = Kexp(-r*T)" << endl;
    cout <<  initial_stock_price << " + " << put_price << " - " << call_price;
    cout << " = " << strike_price << "exp(-" << risk_free_rate << " * " << expiration_time << ")" << endl;
    cout << initial_stock_price + put_price - call_price << " = " << strike_price*exp(-risk_free_rate*expiration_time) << endl;
    cout << "--------------------------------------" << endl;

}



