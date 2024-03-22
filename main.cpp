/*******************************************
 * File: main.cpp                          *
 * Author: Eric Hansson                    *
 * Date:                            *
 * PURPOSE:       *
 *******************************************/
#include <iostream>
#include <fstream>
#include <list>

#include "Block.hpp"
#include "Grammar.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    int heap_size;
    string input_file;

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

    list<Block*> block_list;
    Block* freeList = new Block("freeList", 0, heap_size, 0);
    block_list.push_back(freeList);
    
    cout << "Free List: " << endl;
    cout << freeList->address() << "(" << freeList->size() << ")" << "[" << freeList->ref_count() << "]" << endl;
    cout << "==================================================================================================" << endl;

    //cout << program(ifile) << endl;

  return 0;
}
