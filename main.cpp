/*******************************************
 * File: main.cpp                          *
 * Author: Eric Hansson                    *
 * Date:                            *
 * PURPOSE:       *
 *******************************************/
#include <iostream>
#include <fstream>

#include "Grammar.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    int block_size;
    string input_file;

    cout << "Please enter the initial freelist (heap) size: ";
    cin >> block_size;

    cout << "Please enter the name of an input file: ";
    cin >> input_file;

    // open input file
    ifstream ifile(input_file);

    // if open was not successful, let user know. 
    if (!ifile){
    cerr << "ERROR: Could not open file:\"" << input_file << "\"" <<endl;
    return -1;
    }

    cout << program(ifile) << endl;

  return 0;
}
