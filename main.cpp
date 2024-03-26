/*******************************************
 * File: main.cpp                          *
 * Author: Eric Hansson                    *
 * Date: 3/26/2024                      
 * PURPOSE: Purpose is to read in a file
 * and execute the code of it. Focusing on
 * freeList, memory allocation, freeing 
 * memory, compressing, and garbage collection
 *******************************************/
#include <iostream>
#include <fstream>
#include <list>

#include "Block.hpp"
#include "Grammar.hpp"

using namespace std;

// A list of block pointers 
list<Block*> block_list;

int main(int argc, char *argv[])
{
    // Varibale to keep track of the size and input file name
    int heap_size;
    string input_file;

    // Prompts
    cout << "Please enter the initial freelist (heap) size: ";
    cin >> heap_size;

    cout << "Please enter the name of an input file: ";
    cin >> input_file;

    // open input file
    ifstream ifile(input_file);

    // if open was not successful, let user know. 
    if (!ifile){
    cerr << "ERROR: Could not open file:\"" << input_file << "\"" <<endl;
    return -1;
    }
   
    // Creates a freelist block, and pushes it to the list
    Block* freeList = new Block(0,heap_size);
    block_list.push_back(freeList);

    // Reads the input file and executes accordingly
    program(ifile);

  return 0;
}
