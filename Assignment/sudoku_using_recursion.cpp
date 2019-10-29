//
//  main.cpp
//  prog2-part1
//
//  Created by Jay Chen on 9/4/19.
//  Copyright © 2019 Jay Chen. All rights reserved.
//


#include <iostream>
#include "sudoku.h"

int main (int argc, char * argv[])
{
    Sudoku x;
    
    std::cout << "Input the file" << std::endl;
    x.read_puzzle(argc, argv);
    std::cout << argv[1] << std::endl;
    x.print_puzzle();
    x.Solve(0,0);
    x.sumsolutions();
    //x.alternate_Solve(0, 0);
//    x.print_puzzle();
    
    return 0;
};
