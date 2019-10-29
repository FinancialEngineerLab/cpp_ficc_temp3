//
//  main.cpp
//  prog1-NQueens part2
//
//  Created by Jay Chen on 9/2/19.
//  Copyright © 2019 Jay Chen. All rights reserved.
//

#include <iostream>
#include "part2_hint.h"
using namespace std;

int main(int argc, const char * argv[]) {
    
    Board x;
    cout<<"Enter n for N-Queens problem"<<endl;
    int board_size ;
    cin >> board_size ;
    x.nQueens(board_size);
    
    return 0;
}
