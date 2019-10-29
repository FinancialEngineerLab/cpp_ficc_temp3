//
//  main.cpp
//  midterm
//
//  Created by Jay Chen on 10/1/19.
//  Copyright © 2019 Jay Chen. All rights reserved.
//

#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>
#include <iomanip>
#include <cstdlib>

#include "lp_lib.h"

using namespace std;

const double ERROR = 1e-10;
int number_of_cash_flows;
vector <double> price_list;
vector <int> maturity_list;
vector <double> yield_to_maturity;
vector <double> duration;
vector <double> convexity;
double debt_obligation_amount;
double time_when_debt_is_due;
vector <double> percentage_of_cash_flow_to_meet_debt_obligation;

vector< vector<double> > cash_flows;

//is cash_flow.size() == maturity?
//https://compass2g.illinois.edu/webapps/blackboard/content/listContent.jsp?course_id=_45061_1&content_id=_3997103_1
//inspired by the python sample code in campass.

double function_of_rate(vector <double> cash_flow, double price, int maturity, double rate)
{
    // calculate the f(r)
    double result = price * pow((1+rate), cash_flow.size());
    for(int t=0; t<cash_flow.size(); t++)
        result= result - (cash_flow[t]* pow((1+rate), cash_flow.size()-1-t));
    return result;
}

double derivative_function(vector <double> cash_flow, double price, int maturity, double rate)
{
    //calculate f'(r)
    double result=cash_flow.size() * price * pow((1+rate), (cash_flow.size()-1));
    for(int t=0; t<cash_flow.size()-1 ; t++)
        result = result - (cash_flow[t] * (cash_flow.size()-t-1) * pow((1+rate), (cash_flow.size()-2-t)));
    return result;
}

double Newton_Raphson(vector <double> cash_flow, double price, int maturity, double rate)
{
    //calculate YTM with newton raphson way
    while ( abs(function_of_rate(cash_flow, price, maturity, rate)) > ERROR )
    {
        rate = rate - function_of_rate(cash_flow, price , maturity, rate)/derivative_function(cash_flow, price, maturity, rate);
    }
    return rate;
}

double get_duration(vector <double> cash_flow, double price, int maturity, double rate)
{
    double duration = 0.0;
    for(int t=0; t<cash_flow.size(); t++)
        duration = duration + (cash_flow[t]*(t+1))/pow((1+rate), (t+1));
    duration = duration/price;
    return duration;
}

double get_convexity(vector <double> cash_flow, double price, int maturity, double rate)
{
    double convexity = 0.0;
    for(int t=0; t<cash_flow.size(); t++)
        convexity= convexity + ((t+1)*(t+2)*cash_flow[t])/pow((1+rate), t+3);
    convexity = convexity/price;
    return convexity;
}

double present_value_of_debt(vector<double> cash_flow)
{
    // compute PV of future debt obligation
    // using the average-value-of-the-YTMs
    double avg_ytm;
    double sum=0.0;
    for(int i=0; i<yield_to_maturity.size(); i++)
        sum = sum + yield_to_maturity[i];
    avg_ytm = sum/yield_to_maturity.size();
    
    double pv=0.0;
    for(int t=0; t<cash_flow.size(); t++)
        pv = pv + cash_flow[t]/pow((1+avg_ytm), (1+t));
    
    return pv;
}

void print_data(char *filename)
{
    cout << "Input File: " << filename << endl;
    cout << "We owe " << debt_obligation_amount << " in " << time_when_debt_is_due << " years" << endl;
    cout << "Number of Cash Flows: " << number_of_cash_flows << endl;
    for (int i = 0; i < number_of_cash_flows; i++)
    {
        cout << "---------------------------" << endl;
        cout << "Cash Flow #" << i+1 << endl;
        cout << "Price = " << price_list[i] << endl;
        cout << "Maturity = " << maturity_list[i] << endl;
        cout << "Yield to Maturity = " << yield_to_maturity[i] << endl;
        cout << "Duration = " << duration[i] << endl;
        cout << "Convexity = " << convexity[i] << endl;
        cout << "Percentage of Face Value that would meet the obligation = " <<
        percentage_of_cash_flow_to_meet_debt_obligation[i] << endl;
    }
    cout << "---------------------------" << endl;
}

void get_data(char* argv[])
{
    // write the code that reads the data from the file identified
    // on the command-line.
    // reading the input filename from commandline
    
    //read data here.
    double element;
    
    ifstream input_file(argv[1]);
    
    if (input_file.is_open())
    {
        input_file >> number_of_cash_flows;
        //cout << number_of_cash_flows<<endl;
        
        for(int i=0; i < number_of_cash_flows; i++)
        {
            input_file >> element;
            price_list.push_back(element);
            input_file >> element;
            maturity_list.push_back(element);
            
            vector<double> temp;
            for(int j=0; j< maturity_list[i]; j++)
            {
                input_file >> element;
                temp.push_back(element);
            }
            cash_flows.push_back(temp);
            
            //calculate YTM, Duration, Convexity, percent of cash flow to meed debt and pushback these values in the vectors.
            double ytm;
            ytm = Newton_Raphson(cash_flows[i], price_list[i], maturity_list[i], 0.0);
            yield_to_maturity.push_back(ytm);
            
            double dur;
            dur = get_duration(cash_flows[i], price_list[i], maturity_list[i], ytm);
            duration.push_back(dur);
            
            double cvx;
            cvx = get_convexity(cash_flows[i], price_list[i], maturity_list[i], ytm);
            convexity.push_back(cvx);
           
            double pct;
            pct = 1000/price_list[i];
            percentage_of_cash_flow_to_meet_debt_obligation.push_back(pct);
            
        }
        
        input_file >> debt_obligation_amount;
        input_file >> time_when_debt_is_due;

    }
}

void get_optimal_portfolio()
{
    lprec *lp;
    // write the lp_solve specific material that
    // computes the optimal_portfolio
    
    //solution set
    REAL solution[number_of_cash_flows];
    
    //set row=0, and add row thereafter. set the verbose to 3
    lp=make_lp(0, number_of_cash_flows);
    set_verbose(lp, 3);
    
    //first row is always 0
    double row[number_of_cash_flows+1];
    row[0]=0;
    
    //add duration constraint
    for(int i=0; i<number_of_cash_flows+1; i++)
        row[i+1]= duration[i];
    add_constraint(lp, row, EQ, time_when_debt_is_due);
    
    //add coefficient constraint
    for(int i=0; i<number_of_cash_flows+1; i++)
        row[i+1]= 1;
    add_constraint(lp, row, EQ, 1);
    
    //minimize the convexity
    for(int i=0; i<number_of_cash_flows+1; i++)
        row[i+1]= -convexity[i];
    set_obj_fn(lp, row);
    
    print_lp(lp);
    
    //if solve(lp)==0, we solve the equations successfully
    //then we cout the outcomes.
    
    if(solve(lp)==0)
    {
        cout<<"The lowest Convexity we can get is: "<< -1*get_objective(lp) << endl;
        get_variables(lp, solution);
        cout<<"Optimal portfolio: "<<endl;
        
        for (int i = 0; i < number_of_cash_flows; i++)
            cout <<"%Cash Flow: "<<i+1<<"  " << solution[i] << endl;
        cout<<"*****************************************************************"<<endl;
        cout<<"To immunize against small changes in interest rate for each $1 of PV, you should buy: "<<endl;
        
        //only cout the non-zero solution. which is the combination of the portfoilo
        for (int i = 0; i < number_of_cash_flows; i++)
        {
            if(solution[i]!=0)
                cout<<"$"<<solution[i]<<" of Cash Flow#"<<i+1<<endl;
        }
        cout<<"If you need to immunize for a larger PV-value, just buy an appropriate proportion"<<endl;
        cout<<"*****************************************************************"<<endl;
        cout<<"For example, if you want to immunize for $500 of PV, buy"<<endl;
        for (int i = 0; i < number_of_cash_flows; i++)
        {
            if(solution[i]!=0)
                cout<<"$"<<solution[i]*500<<" of Cash Flow#"<<i+1<<endl;
        }
        cout<<"*****************************************************************"<<endl;
        cout<<"For example, if you want to immunize for $750 of PV, buy"<<endl;
        for (int i = 0; i < number_of_cash_flows; i++)
        {
            if(solution[i]!=0)
                cout<<"$"<<solution[i]*750<<" of Cash Flow#"<<i+1<<endl;
        }
        cout<<"*****************************************************************"<<endl;
        cout<<"For example, if you want to immunize for $1000 of PV, buy"<<endl;
        for (int i = 0; i < number_of_cash_flows; i++)
        {
            if(solution[i]!=0)
                cout<<"$"<<solution[i]*1000<<" of Cash Flow#"<<i+1<<endl;
        }
        cout<<"*****************************************************************"<<endl;
        cout<<"For example, if you want to immunize for $1009.36 of PV, buy"<<endl;
        for (int i = 0; i < number_of_cash_flows; i++)
        {
            if(solution[i]!=0)
                cout<<"$"<<solution[i]*1009.36<<" of Cash Flow#"<<i+1<<endl;
        }
        cout<<"*****************************************************************"<<endl;
    }
    else{
        cout<< "There is no portfolio that meets the duration constraint of "<<time_when_debt_is_due<<" years"<<endl;
    }
    
}
    
int main (int argc, char* argv[])
{
    if (argc == 1) {
        cout << "Input filename missing" << endl;
    }
    else
    {
        get_data(argv);
        
        print_data(argv[1]);
        
        get_optimal_portfolio();
        
        /*
        for(int i=0; i<cash_flows.size(); i++)
        {
            for(int j=0; j<cash_flows[i].size(); j++)
                cout<<cash_flows[i][j]<<" ";
            cout<<endl;
        }
         */

    }
    return (0);
}
