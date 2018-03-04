/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: Tom
 * Project idea from http://www.cplusplus.com/forum/lounge/2299/
 * Created on 26 February 2018, 20:30
 */

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <list>
#include <algorithm>
#include <set>


using namespace std;

/*
 * 
 */
vector< vector<int> > construct_puzzle(){
    vector< vector<int> > rows;
    // Feels very manual, must be a nicer way?
    // TODO: Read in start point from excel sheet
    
    // 0's represent empty boxes
    vector<int> row1 = {0,0,7,0,0,1,3,0,0};
    rows.push_back(row1);
    vector<int> row2 = {3,0,0,0,5,0,6,0,0};
    rows.push_back(row2);
    vector<int> row3 = {5,0,8,0,3,0,0,0,4};
    rows.push_back(row3);
    vector<int> row4 = {0,8,0,0,6,0,9,0,0};
    rows.push_back(row4);
    vector<int> row5 = {7,0,0,0,0,0,0,0,6};
    rows.push_back(row5);
    vector<int> row6 = {0,0,4,0,1,0,0,0,0};
    rows.push_back(row6);
    vector<int> row7 = {2,0,0,0,8,0,4,0,3};
    rows.push_back(row7);
    vector<int> row8 = {0,0,6,0,7,0,0,0,8};
    rows.push_back(row8);
    vector<int> row9 = {0,0,5,9,0,0,7,0,0};
    rows.push_back(row9);
    
    return rows;
}

vector<int> get_related_vertical(vector< vector<int> > puzzle, int x, int y, vector<int> related){
    // Find any used numbers in vertical line
    for (int it = 0; it < 9; it++){
        int val = puzzle[it][x];
        if (val != 0) related.push_back(val);
    }
    return related;
}

vector<int> get_related_horizontal(vector< vector<int> > puzzle, int x, int y, vector<int> related) {
    // Find any used numbers in horizontal line
    for (int it = 0; it < 9; it++){
        int val = puzzle[y][it];
        if (val != 0) related.push_back(val);
    }
    return related;
}

vector<int> get_related_quadrant(vector< vector<int> > puzzle, int x, int y, vector<int> related) {
    // Find any used number in quadrant
    
    // Work out which quadrant the co-ordinates are in
    int x_sector = x/3 * 3;
    int y_sector = y/3 * 3;
    
    for (int y_it = y_sector; y_it < y_sector + 3; y_it++){
        for (int x_it = x_sector; x_it < x_sector + 3; x_it++) {
            int val = puzzle[y_it][x_it];
            if (val != 0) related.push_back(val);     
        }
    }
    return related;
}

void print_vector(vector<int> vec) {
    for (auto & element : vec) {
        cout<<element<<", ";
    }
    cout<<endl;
}

void print_puzzle(vector< vector<int> > rows){
    
    // Create iterators for the in(ner) and ex(ternal) vectors
    vector< vector<int> >::iterator it_ex;
    vector<int>::iterator it_in;
    
    //Print out current game state in grid
    for (it_ex = rows.begin(); it_ex != rows.end(); it_ex++){
        int row_it = 1;
        for (it_in = (*it_ex).begin(); it_in != (*it_ex).end(); it_in++){
            cout << "|";
            // 0's represent empty boxes
            if ((*it_in) == 0) cout << " ";
            // Player 1 is "x"'s
            else cout << *it_in;
            if (row_it == 9) cout << "|" << endl;
            row_it++;
        }
    }
}

int check_for_value_update(vector<int> used, int initial_value) {
    int value;
    int set_size = used.size();
    // Start with list of options for what square could be
    int options[] = {1,2,3,4,5,6,7,8,9};
    if (set_size == 8){
        
        // Convert vector to array to use set_difference
        int* arr = &used[0];
        vector<int> v(9);
        vector<int>::iterator it;
        
        // Work out which option is still available
        it = set_difference (options, options+9, arr, arr+8, v.begin());
        v.resize(it-v.begin());
        
        // Set value to calculated one
        value = v[0];
    } else {
        value = initial_value;
    }
    return value;
}

int check_box(vector< vector<int> > puzzle, int x, int y) {
    // Returns the value for a box
    // Takes the board and the coordinates of the box to checked as params
    
    // vector containing the values already used in other boxes
    vector<int> used;
    
    // Work out which options it couldn't possibly be
    used = get_related_quadrant(puzzle, x, y, used);
    used = get_related_horizontal(puzzle, x, y, used);
    used = get_related_vertical(puzzle, x, y, used);
    
    // Get default value
    int initial_value = puzzle[y][x];
    
    // Get list of unique used numbers
    // TODO: Move to separate function?
    sort( used.begin(), used.end() );
    used.erase( unique( used.begin(), used.end() ), used.end() );
    
    // Work out what the new box value is
    int value = check_for_value_update(used, initial_value);

    return value;
}

vector< vector<int> > solve_puzzle(vector< vector<int> > puzzle){
    // Takes the original partial board and returns the completed one
    // Need to cycle through each box attempting  to solve them
    
    // Use the number of 0's to work out when puzzle is completed
    int num_zeros = 1;
    while (num_zeros != 0) {
        // Create iterators for the in(ner) and ex(ternal) vectors
        vector< vector<int> >::iterator it_ex;
        vector<int>::iterator it_in;
        int col_it = 0;
        num_zeros = 0;
        
        // Iterate through vector
        for (it_ex = puzzle.begin(); it_ex != puzzle.end(); it_ex++){
            int row_it = 0;
            for (it_in = (*it_ex).begin(); it_in != (*it_ex).end(); it_in++){
                //If box already has a non-zero value there is no need to re-check
                if (*it_in == 0) {
                    num_zeros += 1;
                    
                    // Attempt to find solution for a box
                    int answer = check_box(puzzle, row_it, col_it);
                    *it_in = answer;
                    
                    // If the value is updated print the new puzzle
                    if (answer != 0) {
                        cout<<"------------------"<<endl;
                        print_puzzle(puzzle);
                    }
                }
                row_it++;
            }
            col_it++;
        }
    }
    return puzzle;
}

int main(int argc, char** argv) {
    // Construct the pre-determined puzzle
    vector< vector<int> > puzzle = construct_puzzle();
    print_puzzle(puzzle);
    
    // Solve the puzzle
    // Note that currently solving relies of process of elimination on a
    // square by square basis which does not work for all puzzles 
    // or even this one :(
    // TODO: Update puzzle to work out values based on what number can't go
    // anywhere else
    vector< vector<int> > solved_puzzle = solve_puzzle(puzzle);
    
    // Print out the completed puzzle
    print_puzzle(solved_puzzle);
    
    return 0;
}

