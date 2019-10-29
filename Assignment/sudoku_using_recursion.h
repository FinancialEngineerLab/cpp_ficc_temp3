//
//  hint.h
//  prog2-part1
//
//  Created by Jay Chen on 9/4/19.
//  Copyright © 2019 Jay Chen. All rights reserved.
//

#ifndef sudoku
#define sudoku

#include <vector>
#include <fstream>
using std::vector;
using namespace std;

class Sudoku
{
    // Private
    int puzzle[9][9] ;
    int count=0;
    // Private member function that checks if the named row is valid
    //p[i][j] = k;
    bool row_valid(int row, int col, int k)
    {
        // write code that checks if "row" is valid
        for(int c=0; c<9; c++)
            if(puzzle[row][c] == k && c!=col)
                return false;
        return true;
    }
    
    // Private member function that checks if the named column is valid
    bool col_valid(int row, int col, int k)
    {
        // check validity of "col"
        for(int r=0; r<9; r++)
            if(puzzle[r][col] == k && r != row)
                return false;
        return true;
    }
    
    // Private member function that checks if the named 3x3 block is valid
    bool block_valid(int row, int col, int k)
    {
        // check 3 x 3 block validity
        //basically i use the most stupid way to check the block valid.
        //first, i check the row&col to make sure which small block the position in. later, in different blocks, just go through it and check.
        if(row<=2 && col<=2)
            for(int r=0; r<3; r++)
                for(int c=0; c<3; c++)
                    if(puzzle[r][c]== k && r!=row && c!=col)
                        return false;
        if(row>2 && row<=5 && col<=2)
            for(int r=3; r<6; r++)
                for(int c=0; c<3; c++)
                    if(puzzle[r][c]== k && r!=row && c!=col)
                        return false;
        if(row>5 && col<=2)
            for(int r=6; r<9; r++)
                for(int c=0; c<3; c++)
                    if(puzzle[r][c]== k && r!=row && c!=col)
                        return false;
        if(row<=2 && col>=3 && col<=5)
            for(int r=0; r<3; r++)
                for(int c=3; c<6; c++)
                    if(puzzle[r][c]== k && r!=row && c!=col)
                        return false;
        if(row<=2 && col>=6)
            for(int r=0; r<3; r++)
                for(int c=6; c<9; c++)
                    if(puzzle[r][c]== k && r!=row && c!=col)
                        return false;
        if(row>2 && row<=5 && col>2 && col<=5)
            for(int r=3; r<6; r++)
                for(int c=3; c<6; c++)
                    if(puzzle[r][c]== k && r!=row && c!=col)
                        return false;
        if(row>=6 && col>2 && col<=5)
            for(int r=6; r<9; r++)
                for(int c=3; c<6; c++)
                    if(puzzle[r][c]== k && r!=row && c!=col)
                        return false;
        if(row>2 && row<=5 && col>5 )
            for(int r=3; r<6; r++)
                for(int c=6; c<9; c++)
                    if(puzzle[r][c]== k && r!=row && c!=col)
                        return false;
        if(row>5 && col>5)
            for(int r=6; r<9; r++)
                for(int c=6; c<9; c++)
                    if(puzzle[r][c]== k && r!=row && c!=col)
                        return false;
        return true;
    }
    
    //combine the three restriction
    bool check_all_restrictions(int row, int col, int k)
    {
        if(row_valid(row, col, k) && col_valid(row, col, k) && block_valid(row, col, k))
            return true;
        return false;
    }
    
    //find the next position which is ‘0’, and change the row&col so that can go to the next position.
    bool check_next_blank(int &row, int &col)
    {
        for(row=0; row< 9; row++)
            for( col=0; col<9; col++ )
                if(puzzle[row][col]==0)
                    return true;
        return false;
    }
    
public:
    // Public member function that reads the incomplete puzzle
    // we are not doing any checks on the input puzzle -- that is,
    // we are assuming they are indeed valid
    //read the puzzle.
    void read_puzzle(int argc, char* argv[])
    {
        ifstream input_file(argv[1]);
        if (input_file.is_open())
        {
                for (int i = 0; i < 9; i++)
                    for (int j = 0; j < 9; j++)
                       input_file >> puzzle[i][j];
                input_file.close();
        }
        else
            cout << "Input file " << argv[1] << " does not exist in PWD" << endl;
    }
    
    // Public member function that prints the puzzle when called
    void print_puzzle()
    {
        cout << endl << "Board Position " << count+1 << endl;
        for (int i = 0; i < 9; i++)
        {
            for (int j = 0; j < 9; j++)
            {
                // check if we have a legitimate integer between 1 and 9
                if ((puzzle[i][j] >= 1) && (puzzle[i][j] <= 9))
                {
                    // printing initial value of the puzzle with some formatting
                    cout << puzzle[i][j] << " ";
                }
                else {
                    // printing initial value of the puzzle with some formatting
                    cout << "X ";
                }
            }
            cout << endl;
        }
    }
    
    // Public member function that (recursively) implements the brute-force
    // search for possible solutions to the incomplete Sudoku puzzle
    void Solve(int row, int col)
    {
        // this part of the code identifies the row and col number of the
        // first incomplete (i.e. 0) entry in the puzzle.  If the puzzle has
        // no zeros, the variable row will be 9 => the puzzle is done, as
        // each entry is row-, col- and block-valid...
        
        // use the pseudo code of figure 3 of the description
        //In this part, i use the pseudo code and the thought of solution to the Homework1. they are basically the same, so i use the same way to solve it.
        
        if(!check_next_blank(row, col))
        {
            //find the next '0' position, and change row&col to that position
            print_puzzle();
            count++;
        }
        //this part is basically the same as the last assignment
        for(int k=1; k<=9; k++)
        {
            if(check_all_restrictions(row, col, k))
            {
                puzzle[row][col] = k;
                Solve(row,col);
                puzzle[row][col]=0;
            }
        }
    }
    void sumsolutions()
    {
        cout<< "There are "<< count << " solutions"<<endl;
    }
};

#endif
