//
//  main.cpp
//  endterm prog p2
//
//  Created by Jay Chen on 12/2/19.
//  Copyright © 2019 Jay Chen. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include <cmath>
#include <fstream>
#include <cstdlib>
#include <random>
#include <chrono>
using namespace std;

double risk_free_rate, strike_price, initial_stock_price, expiration_time, volatility, barrier_price;
int no_of_trials, no_of_barriers;

unsigned seed = (unsigned) std::chrono::system_clock::now().time_since_epoch().count();
std::default_random_engine generator (seed);

double get_uniform()
{
    std::uniform_real_distribution <double> distribution(0.0,1.0);
    double number = distribution(generator);
    return (number);
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


int main(int argc, const char * argv[]) {
    sscanf (argv[1], "%lf", &expiration_time);
    sscanf (argv[2], "%lf", &risk_free_rate);
    sscanf (argv[3], "%lf", &volatility);
    sscanf (argv[4], "%lf", &initial_stock_price);
    sscanf (argv[5], "%lf", &strike_price);
    sscanf (argv[6], "%d",  &no_of_trials);
    sscanf (argv[7], "%d",  &no_of_barriers);
    sscanf (argv[8], "%lf", &barrier_price);
    
    const float delta_T = expiration_time/((float) no_of_barriers);
    const float delta_R = (risk_free_rate - 0.5*pow(volatility,2))*delta_T;
    const float delta_SD = volatility*sqrt(delta_T);
    
    cout << "--------------------------------" << endl;
    cout<<"European Down-and-out Discrete Barrier Options Pricing via Monte Carlo Simulation"<<endl;
    cout << "--------------------------------" << endl;
    cout << "Expiration Time (Years) = " << expiration_time << endl;
    cout << "Risk Free Interest Rate = " << risk_free_rate << endl;
    cout << "Volatility (%age of stock value) = " << volatility*100 << endl;
    cout << "Initial Stock Price = " << initial_stock_price << endl;
    cout << "Strike Price = " << strike_price << endl;
    cout << "Barrier Price = " << barrier_price << endl;
    cout << "Number of Trials = " << no_of_trials << endl;
    cout << "Number of Discrete Barriers = " << no_of_barriers << endl;
    cout << "--------------------------------" << endl;

    double put_option_price = 0.0;
    double call_option_price = 0.0;
    double put_option_price_adj = 0.0;
    double call_option_price_adj = 0.0;
    
    for (int i = 0; i < no_of_trials; i++)
    {
        float current_stock_price1 = initial_stock_price;
        float current_stock_price2 = initial_stock_price;
        float current_stock_price3 = initial_stock_price;
        float current_stock_price4 = initial_stock_price;
        
        float current_stock_price1_adj = initial_stock_price;
        float current_stock_price2_adj = initial_stock_price;
        float current_stock_price3_adj = initial_stock_price;
        float current_stock_price4_adj = initial_stock_price;
        
        float p_d1=1.0; float p_d2=1.0; float p_d3=1.0; float p_d4=1.0;
        
        for(int j=0; j < no_of_barriers; j++)
        {
            // create the unit normal variates using the Box-Muller Transform
            float x = get_uniform();
            float y = get_uniform();
            float a =  sqrt(-2.0*log(x)) * cos(6.283185307999998*y);
            float b =  sqrt(-2.0*log(x)) * sin(6.283185307999998*y);
            
            // check if the current_stock_price > barrier price ?
            current_stock_price1 = ((current_stock_price1*exp(delta_R + delta_SD*a)) > barrier_price) ? (current_stock_price1*exp(delta_R + delta_SD*a)) :0 ;
            current_stock_price2 = ((current_stock_price2*exp(delta_R - delta_SD*a)) > barrier_price) ? (current_stock_price2*exp(delta_R - delta_SD*a)) :0 ;
            current_stock_price3 = ((current_stock_price3*exp(delta_R + delta_SD*b)) > barrier_price) ? (current_stock_price3*exp(delta_R + delta_SD*b)) :0 ;
            current_stock_price4 = ((current_stock_price4*exp(delta_R - delta_SD*b)) > barrier_price) ? (current_stock_price4*exp(delta_R - delta_SD*b)) :0 ;
            
            current_stock_price1_adj = current_stock_price1_adj*exp(delta_R+delta_SD*a);
            current_stock_price2_adj = current_stock_price2_adj*exp(delta_R-delta_SD*a);
            current_stock_price3_adj = current_stock_price3_adj*exp(delta_R+delta_SD*b);
            current_stock_price4_adj = current_stock_price4_adj*exp(delta_R-delta_SD*b);
        
            float mean1 = (initial_stock_price + (((float) j/ (float) no_of_barriers )) * (current_stock_price1_adj - initial_stock_price)) ;
            float mean2 = (initial_stock_price + (((float) j/ (float) no_of_barriers )) * (current_stock_price2_adj - initial_stock_price)) ;
            float mean3 = (initial_stock_price + (((float) j/ (float) no_of_barriers )) * (current_stock_price3_adj - initial_stock_price)) ;
            float mean4 = (initial_stock_price + (((float) j/ (float) no_of_barriers )) * (current_stock_price4_adj - initial_stock_price)) ;
            
            float std = sqrt( (((expiration_time/(float) no_of_barriers)* (float) j) * (1 - ((float) j / (float) no_of_barriers))) );
            
            p_d1 *= ( 1- N((barrier_price - mean1)/std) );
            p_d2 *= ( 1- N((barrier_price - mean2)/std) );
            p_d3 *= ( 1- N((barrier_price - mean3)/std) );
            p_d4 *= ( 1- N((barrier_price - mean4)/std) );
            
        }
        
        double v1 = (current_stock_price1 != 0) ? 1:0;
        double v2 = (current_stock_price2 != 0) ? 1:0;
        double v3 = (current_stock_price3 != 0) ? 1:0;
        double v4 = (current_stock_price4 != 0) ? 1:0;
        
        call_option_price +=
                   (max(0.0, current_stock_price1 - strike_price)*v1 +
                    max(0.0, current_stock_price2 - strike_price)*v2 +
                    max(0.0, current_stock_price3 - strike_price)*v3 +
                    max(0.0, current_stock_price4 - strike_price)*v4)/4.0;
        
        put_option_price +=
                  (max(0.0, strike_price - current_stock_price1)*v1 +
                   max(0.0, strike_price - current_stock_price2)*v2 +
                   max(0.0, strike_price - current_stock_price3)*v3 +
                   max(0.0, strike_price - current_stock_price4)*v4)/4.0;
       
        double av1 = (current_stock_price1_adj > barrier_price) ? 1:0;
        double av2 = (current_stock_price2_adj > barrier_price) ? 1:0;
        double av3 = (current_stock_price3_adj > barrier_price) ? 1:0;
        double av4 = (current_stock_price4_adj > barrier_price) ? 1:0;
        
        call_option_price_adj +=
                   (max(0.0, current_stock_price1_adj - strike_price)*p_d1*av1 +
                    max(0.0, current_stock_price2_adj - strike_price)*p_d2*av2 +
                    max(0.0, current_stock_price3_adj - strike_price)*p_d3*av3 +
                    max(0.0, current_stock_price4_adj - strike_price)*p_d4*av4)/4.0;
        
        put_option_price_adj +=
                  (max(0.0, strike_price - current_stock_price1_adj)*p_d1*av1 +
                   max(0.0, strike_price - current_stock_price2_adj)*p_d2*av2 +
                   max(0.0, strike_price - current_stock_price3_adj)*p_d3*av3 +
                   max(0.0, strike_price - current_stock_price4_adj)*p_d4*av4)/4.0;
    }
    
    call_option_price = exp(-risk_free_rate*expiration_time)*(call_option_price/((double) no_of_trials));
    put_option_price = exp(-risk_free_rate*expiration_time)*(put_option_price/((double) no_of_trials));
    
    call_option_price_adj = exp(-risk_free_rate*expiration_time) * (call_option_price_adj/((double) no_of_trials)) ;
    put_option_price_adj = exp(-risk_free_rate*expiration_time) * (put_option_price_adj/((double) no_of_trials)) ;
    
    
    
    cout << "--------------------------------" << endl;
    cout<<"The average Call Price by explict simulation of price paths           = "<< call_option_price << endl;
    cout<<"The average Call Price with Brownian-Bridge Correction on final price = "<< call_option_price_adj << endl;
   
    
    
    cout << "--------------------------------" << endl;
    cout<<"The average Put Price by explict simulation of price paths           = "<< put_option_price << endl;
    cout<<"The average Put Price with Brownian-Bridge Correction on final price = "<< put_option_price_adj << endl;
    
    return 0;
}

