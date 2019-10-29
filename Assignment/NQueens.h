//
//  Header.h
//  prog1-NQueens part2
//
//  Created by Jay Chen on 9/2/19.
//  Copyright © 2019 Jay Chen. All rights reserved.
//


#ifndef part2_hint_h
#define part2_hint_h
#include <iostream>

using namespace std;

class Board
{
    // private data member: size of the board
    int size;
    
    // pointer-to-pointer initialization of the board
    int **chess_board = new int*[size] ;
    
    
    int sum_solutions = 0;
     
    // private member function: initializes the (n x n) chessboard
    void initialize(int n)
    {
        size = n;
        
        // write the appropriate code that uses the pointer-to-pointer
        // method to initialize the (n x n) chessboard.  Once initialized,
        // put zeros in all entries.  Later on, if you placed a queen in
        // the (i,j)-th position, then chessboard[i][j] will be 1.
        for(int i = 0; i < size; i++)
            chess_board[i] = new int[size];
        
        for(int i = 0; i < n ; i++  )
        {
            for(int j = 0; j < n ; j++)
            {
                chess_board[i][j] = 0;
            }
        }
    }
    
    // private member function:  returns 'false' if
    // the (row, col) position is not safe.
    bool is_this_position_safe(int row, int col)
    {
        // write the appropriate code on your own that returns
        // "true" if the (row,col) position is safe.  If it is
        // unsafe (i.e. some other queen can threaten this position)
        // return "false"
        int i,j;
        //check if the col is safe
        for(  i = 0 ; i < size ; i++)
            if( chess_board[i][col] == 1 )
                return false;
        //check if the row is safe
        for(  j = 0 ; j < size ; j++)
            if( chess_board[row][j] == 1 )
                return false;
        //to check the digonal
        //I go through all 4 diff dirctions in digonal to see if there is QUEEN on the digonal.but then i delete two part because I don't have to check a position if i have not put a queen in there.
        for (i = row, j = col; i >= 0 && j >= 0; i--, j--)
            if (chess_board[i][j] == 1)
                return false;
//        for (i = row, j = col; i < size && j < size; i++, j++)
//            if (chess_board[i][j] == 1)
//                return false;
        for (i = row, j = col; j >= 0 && i < size; i++, j--)
            if (chess_board[i][j] == 1)
                return false;
//        for (i = row, j = col; j < size && i >= 0; i--, j++)
//            if (chess_board[i][j] == 1)
//                return false;
        
        //if the position is safe, return true
        return true;
        
    }
    // private member function: prints the board position
    void print_board()
    {
        cout << size << "-Queens Problem Solutions - NO." <<sum_solutions+1 <<endl;
        // write the appropriate code here to print out the solved
        // board as shown in the assignment description
        for(int row = 0; row < size; row++)
        {
            for(int col = 0; col < size; col++)
                cout<< chess_board[row][col] << " ";
            cout<< endl;
        }
    }
    
    //I write this part because I think all the solutions are symmatrical. when you found one solution, actually you find twe by symmatric.
//    void print_row_symmatrical_board()
//    {
//        cout << size << "-Queens Problem Solutions - NO." <<sum_solutions+2 <<endl;
//        for(int row = size-1; row >= 0; row--)
//        {
//            for(int col = 0; col < size; col++)
//                cout<< chess_board[row][col] << " ";
//            cout<< endl;
//        }
//    }
    
    // private member function: recursive backtracking
    void solve(int col)
    {
        // implement the recursive backtracking procedure described in
        // pseudocode format in figure 1 of the description of the first
        // programming assignment
        
        //for this part, i get inspired by https://www.geeksforgeeks.org/printing-solutions-n-queen-problem/ and I fully understand what's going on in this recursion.
        if (col >= size)
        {
            //if col>=size means the program find a solution,therefore we need to print this solution and count the solutions.
            print_board();
            sum_solutions = sum_solutions +1;
        }
        
        for(int row = 0; row< size ; row++ )
        {
            if( is_this_position_safe(row, col) == true )
            {
                //if the position is safe, put queen there, and try to find next solution.
                //actually i found that I don't have to return something to keep the recurrsion, so i used the viod type function.
                chess_board[row][col] = 1;
                //after checking first col, checking the next. and keep recurrsion
                solve(col+1);
                //after finding a solution and print, backtracking, move the queen.
                chess_board[row][col] = 0;
                
            }
        }
    };
    
public:
    // Solves the n-Queens problem by (recursive) backtracking
    void nQueens(int n)
    {
        initialize(n);
        solve(0)  ;
        cout<<"There are "<<sum_solutions<<" solutions to solve "<<n << "-Queens Problem" << endl;
        
    }
};




#endif /* hint_h */
