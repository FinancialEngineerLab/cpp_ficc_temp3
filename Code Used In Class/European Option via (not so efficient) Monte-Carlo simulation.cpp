// Pricing an European Option using Simulation
// Written by Prof. Sreenivas
#include <iostream>
#include <iomanip>
#include <cmath>
#include <fstream>
#include <cstdlib>
#include "black_scholes.h" 
using namespace std;

double risk_free_rate, strike_price, initial_stock_price, expiration_time, volatility;
int no_of_trials;

int main (int argc, char* argv[])
{	
	sscanf (argv[1], "%lf", &expiration_time);
	sscanf (argv[2], "%lf", &risk_free_rate);
	sscanf (argv[3], "%lf", &volatility);
	sscanf (argv[4], "%lf", &initial_stock_price);
	sscanf (argv[5], "%lf", &strike_price);
	sscanf (argv[6], "%d", &no_of_trials);
	
	double R = (risk_free_rate - 0.5*pow(volatility,2))*expiration_time;
	double SD = volatility*sqrt(expiration_time); 
	
	cout << "--------------------------------" << endl;
	cout << "European Options Pricing via Monte Carlo Simulation" << endl;
	cout << "Expiration Time (Years) = " << expiration_time << endl;
	cout << "Risk Free Interest Rate = " << risk_free_rate << endl;
	cout << "Volatility (%age of stock value) = " << volatility*100 << endl;
	cout << "Initial Stock Price = " << initial_stock_price << endl;
	cout << "Strike Price = " << strike_price << endl;
	cout << "Number of Trials = " << no_of_trials << endl;
	cout << "--------------------------------" << endl;
	
	double put_option_price = 0.0;
	double call_option_price = 0.0;
	for (int i = 0; i < no_of_trials; i++) 
	{
		double S_T = initial_stock_price * exp(R + SD*get_gaussian());
		call_option_price += max(0.0, S_T - strike_price);
		put_option_price += max(0.0, strike_price - S_T);
	}
	
	call_option_price = exp(-risk_free_rate*expiration_time)*(call_option_price/((double) no_of_trials));
	put_option_price = exp(-risk_free_rate*expiration_time)*(put_option_price/((double) no_of_trials));
	
	cout << "The average Call Price is " << call_option_price << endl;
	cout << "Call Price according to Black-Scholes = " <<
	option_price_call_black_scholes(initial_stock_price, strike_price, risk_free_rate, 
									volatility, expiration_time) << endl;
	cout << "The average Put Price is " << put_option_price << endl;
	cout << "Put Price according to Black-Scholes = " <<
	option_price_put_black_scholes(initial_stock_price, strike_price, risk_free_rate, 
								   volatility, expiration_time) << endl;
	cout << "--------------------------------" << endl;
}	




